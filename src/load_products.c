#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "final_project.h"

typedef struct {
    int product_id;
    int book_id;
    char isbn[100];
    char subject[100];
    char author[100];
    char publisher[100];
    char translator[100];
    int weight;
    int page_no;
    char cover_type[100];
    int cover_no;
    char format[100];
    char suitable_for[100];
    char description[1000];
} Book;

typedef struct {
    int product_id;
    int puzzle_id;
    int weight;
    char puzzle[100];
    char size[100];
    char type[100];
    char packet_size[100];
    int packet_weight;
    char producer[100];
    char content[100];
    char description[1000];
    char ages[100];
    int risk_of_devouring;
} Puzzle;

typedef struct {
    int product_id;
    int keyboard_id;
    char size[100];
    char cable_type[100];
    char os_compatible[100];
    int weight;
    int backround_light;
    int shortcut_key_no;
    char mice_range[100];
    char accuracy[100];
    int fa_letter;
    int keys_no;
    int dustproof;
    char power[100];
    char cable_len[100];
    char conn_type[100];
    int usb_input;
} Keyboard;

typedef struct {
    int product_id;
    int mice_id;
    char os_compatible[100];
    char cable_len[100];
    char size[100];
    char sensor_type[100];
    char conn_type[100];
    char accuracy_range[100];
    int weight;
    int two_hand_used;
    char cable_type[100];
} Mice;

typedef struct {
    int product_id;
    int screen_guard_id;
    char type[100];
    char compatible_with[400];
    int easy_to_use;
    char thickness[100];
    int scratches_prevent;
    int strike_prevent;
    int light_protecting;
    char other_attr[2000];
} ScreenGuard;

typedef struct {
    int product_id;
    int phone_cover_id;
    char compatible_with[100];
    int weight;
    char type[100];
    char size[100];
    char structure[100];
    char material[100];
    char special_attr[500];
    char cover_surface[100];
} PhoneCover;

typedef struct {
    char id[15];
    char product_title_fa[500];
    char product_title_en[500];
    char url_code[500];
    char title_alt[1000];
    char category_title_fa[200];
    char category_keywords[1000];
    char brand_name_fa[200];
    char brand_name_en[200];
    char product_attr[10000];
} TempProduct;


static void parse_csv(char *line, TempProduct *product, const char *sep) {
    int i = 0;
    char *token;
    while ((token = strsep(&line, sep)) != NULL) {
        switch (i++) {
            case 0:
                strcpy(product->id, token);
                break;
            case 1:
                strcpy(product->product_title_fa, token);
                break;
            case 2:
                strcpy(product->product_title_en, token);
                break;
            case 3:
                strcpy(product->url_code, token);
                break;
            case 4:
                strcpy(product->title_alt, token);
                break;
            case 5:
                strcpy(product->category_title_fa, token);
                break;
            case 6:
                strcpy(product->category_keywords, token);
                break;
            case 7:
                strcpy(product->brand_name_fa, token);
                break;
            case 8:
                strcpy(product->brand_name_en, token);
                break;
            case 9:
                strcpy(product->product_attr, token);
                break;
            default:
                break;

        }
    }
}

int insert_category(TempProduct *product) {
    char insert_cmd[4000], select_cmd[4000];
    sprintf(insert_cmd,
            "INSERT INTO category (name_fa, keywords) SELECT \"%s\",\"%s\" FROM DUAL WHERE NOT EXISTS (SELECT * FROM category WHERE name_fa LIKE \"%%%s%%\");",
            product->category_title_fa, product->category_keywords, product->category_title_fa);
    sprintf(select_cmd, "SELECT category_id FROM category WHERE name_fa LIKE \"%%%s%%\";", product->category_title_fa);
    if (mysql_query(mysql, insert_cmd) != 0 || mysql_query(mysql, select_cmd) != 0)
        return -1;
    MYSQL_RES *result = mysql_store_result(mysql);
    if (result == NULL)
        return -1;
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);
    return atoi(row[0]);
}

int insert_brand(TempProduct *product) {
    char insert_cmd[500], select_cmd[400];
    sprintf(insert_cmd,
            "INSERT INTO brand (name_en, name_fa) SELECT \"%s\",\"%s\" FROM DUAL WHERE NOT EXISTS (SELECT * FROM brand WHERE name_en LIKE \"%%%s%%\" AND name_fa LIKE \"%%%s%%\");",
            product->brand_name_en, product->brand_name_fa, product->brand_name_en, product->brand_name_fa);
    sprintf(select_cmd, "SELECT brand_id FROM brand WHERE name_en LIKE \"%%%s%%\" AND name_fa LIKE \"%%%s%%\";",
            product->brand_name_en, product->brand_name_fa);
    if (mysql_query(mysql, insert_cmd) != 0 || mysql_query(mysql, select_cmd) != 0)
        return -1;
    MYSQL_RES *result = mysql_store_result(mysql);
    if (result == NULL)
        return -1;
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);
    return atoi(row[0]);
}

int insert_product(TempProduct *product, int category_id, int brand_id) {
    char insert_cmd[6000];
    sprintf(insert_cmd,
            "INSERT INTO product (product_id, title_en, title_fa, title_alt, url_code, brand_id, category_id) VALUE (%d, \"%s\", \"%s\",\"%s\",\"%s\",%d,%d);",
            atoi(product->id), product->product_title_en, product->product_title_fa, product->title_alt,
            product->url_code, brand_id, category_id);
    if (mysql_query(mysql, insert_cmd) != 0)
        return -1;
    return 0;
}

int extract_book_attr(TempProduct *product, Book *book) {
    char token[250], attr[8000];
    strcpy(attr, product->product_attr);
    sscanf(attr, "[%[^]]", attr);
    FILE *stream = fmemopen(attr, strlen(attr), "r");
    book->product_id = atoi(product->id);
    while (fscanf(stream, "{%[^}]},", token) != EOF) {
        char key[50] = {0}, value[200] = {0};
        if (strstr(token, "Value") == NULL)
            continue;
        sscanf(token, "\"Key\":\"%[^\"]\",\"Value\":\"%[^\"]\"", key, value);
        if (strcmp(key, "موضوع") == 0) {
            strcpy(book->subject, value);
            continue;
        }
        if (strcmp(key, "ناشر") == 0) {
            strcpy(book->publisher, value);
            continue;
        }
        if (strcmp(key, "مترجم") == 0) {
            strcpy(book->translator, value);
            continue;
        }
        if (strcmp(key, "وزن") == 0) {
            book->weight = atoi(value);
            continue;
        }
        if (strcmp(key, "تعداد صفحات") == 0) {
            book->page_no = atoi(value);
            continue;
        }
        if (strcmp(key, "نوع جلد") == 0) {
            strcpy(book->cover_type, value);
            continue;
        }
        if (strcmp(key, "تعداد جلد") == 0) {
            book->cover_no = atoi(value);
            continue;
        }
        if (strcmp(key, "قطع") == 0) {
            strcpy(book->format, value);
            continue;
        }
        if (strcmp(key, "مناسب برای") == 0) {
            strcpy(book->suitable_for, value);
            continue;
        }
        if (strcmp(key, "سایر توضیحات") == 0) {
            strcpy(book->description, value);
            continue;
        }
        if (strcmp(key, "شابک") == 0) {
            strcpy(book->isbn, value);
            continue;
        }
        if (strcmp(key, "نویسنده\\/نویسندگان") == 0) {
            strcpy(book->author, value);
            continue;
        }
    }
    fclose(stream);
    return 0;
}

int insert_book(Book *book) {
    char insert_cmd[4000];
    sprintf(insert_cmd,
            "INSERT INTO book (product_id, isbn, subject, author, publisher, translator, weight, pages_no, cover_type, cover_no, format, suitable_for, description) VALUE (%d, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %d, %d, \"%s\", %d, \"%s\", \"%s\", \"%s\");",
            book->product_id, book->isbn, book->subject, book->author, book->publisher, book->translator, book->weight,
            book->page_no, book->cover_type, book->cover_no, book->format, book->suitable_for, book->description);
    if (mysql_query(mysql, insert_cmd) != 0) {
        printf("insert book is: %s\n", insert_cmd);
        return -1;
    }
    return 0;
}

int extract_puzzle_attr(TempProduct *product, Puzzle *puzzle) {
    char key[50], value[500], token[550], attr[8000];
    strcpy(attr, product->product_attr);
    sscanf(attr, "[%[^]]", attr);
    FILE *stream = fmemopen(attr, strlen(attr), "r");
    puzzle->product_id = atoi(product->id);
    while (fscanf(stream, "{%[^}]},", token) != EOF) {
        char key[50] = {0}, value[500] = {0};
        if (strstr(token, "Value") == NULL)
            continue;
        sscanf(token, "\"Key\":\"%[^\"]\",\"Value\":\"%[^\"]\"", key, value);
        if (strcmp(key, "پازل") == 0) {
            strcpy(puzzle->puzzle, value);
            continue;
        }
        if (strcmp(key, "ابعاد") == 0) {
            strcpy(puzzle->size, value);
            continue;
        }
        if (strcmp(key, "نوع") == 0) {
            strcpy(puzzle->type, value);
            continue;
        }
        if (strcmp(key, "وزن") == 0) {
            puzzle->weight = atoi(value);
            continue;
        }
        if (strcmp(key, "ابعاد بسته‌بندی") == 0) {
            strcpy(puzzle->packet_size, value);
            continue;
        }
        if (strcmp(key, "وزن بسته‌بندی") == 0) {
            puzzle->packet_weight = atoi(value);
            continue;
        }
        if (strcmp(key, "سازنده") == 0) {
            strcpy(puzzle->producer, value);
            continue;
        }
        if (strcmp(key, "محتویات بسته") == 0) {
            strcpy(puzzle->content, value);
            continue;
        }
        if (strcmp(key, "رده سنی") == 0) {
            strcpy(puzzle->ages, value);
            continue;
        }
        if (strcmp(key, "سایر توضیحات") == 0) {
            strcpy(puzzle->description, value);
            continue;
        }
        if (strcmp(key, "خطر بلعیدن") == 0) {
            if (strcmp(value, "دارد") == 0)
                puzzle->risk_of_devouring = 1;
            else
                puzzle->risk_of_devouring = 0;
            continue;
        }
    }
    fclose(stream);
    return 0;
}

int insert_puzzle(Puzzle *puzzle) {
    char insert_cmd[8000];
    sprintf(insert_cmd,
            "INSERT INTO puzzle (product_id, weight, puzzle, size, type, packet_size, packet_weight, producer, content, description, ages, risk_of_devouring) VALUE (%d, %d, \"%s\", \"%s\", \"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\", \"%s\", %d);",
            puzzle->product_id, puzzle->weight, puzzle->puzzle, puzzle->size, puzzle->type, puzzle->packet_size,
            puzzle->packet_weight, puzzle->producer, puzzle->content, puzzle->description, puzzle->ages,
            puzzle->risk_of_devouring);
    if (mysql_query(mysql, insert_cmd) != 0) {
        printf("insert puzzle is: %s\n", insert_cmd);
        return -1;
    }
    return 0;
}

int extract_keyboard_attr(TempProduct *product, Keyboard *keyboard) {
    char token[550], attr[8000];
    strcpy(attr, product->product_attr);
    sscanf(attr, "[%[^]]", attr);
    FILE *stream = fmemopen(attr, strlen(attr), "r");
    keyboard->product_id = atoi(product->id);
    while (fscanf(stream, "{%[^}]},", token) != EOF) {
        char key[50] = {0}, value[500] = {0};
        if (strstr(token, "Value") == NULL)
            continue;
        sscanf(token, "\"Key\":\"%[^\"]\",\"Value\":\"%[^\"]\"", key, value);
        if (strcmp(key, "منبع تغذیه") == 0) {
            strcpy(keyboard->power, value);
            continue;
        }
        if (strcmp(key, "ابعاد") == 0) {
            strcpy(keyboard->size, value);
            continue;
        }
        if (strcmp(key, "نوع کابل") == 0) {
            strcpy(keyboard->cable_type, value);
            continue;
        }
        if (strcmp(key, "وزن") == 0) {
            keyboard->weight = atoi(value);
            continue;
        }
        if (strcmp(key, "سازگار با سیستم‌عامل‌های") == 0) {
            strcpy(keyboard->os_compatible, value);
            continue;
        }
        if (strcmp(key, "تعداد کلید‌ها") == 0) {
            keyboard->keys_no = atoi(value);
            continue;
        }
        if (strcmp(key, "طول کابل") == 0) {
            strcpy(keyboard->cable_len, value);
            continue;
        }
        if (strcmp(key, "نوع اتصال") == 0) {
            strcpy(keyboard->conn_type, value);
            continue;
        }
        if (strcmp(key, "ورودی USB") == 0) {
            if (strcmp(value, "دارد") == 0)
                keyboard->usb_input = 1;
            else
                keyboard->usb_input = 0;
            continue;
        }
        if (strcmp(key, "حروف حک شده فارسی") == 0) {
            if (strcmp(value, "دارد") == 0)
                keyboard->fa_letter = 1;
            else
                keyboard->fa_letter = 0;
            continue;
        }
        if (strcmp(key, "دقت") == 0) {
            strcpy(keyboard->accuracy, value);
            continue;
        }
        if (strcmp(key, "برد ماوس") == 0) {
            strcpy(keyboard->mice_range, value);
            continue;
        }
        if (strcmp(key, "تعداد کلید‌های میانبر") == 0) {
            keyboard->shortcut_key_no = atoi(value);
            continue;
        }
        if (strcmp(key, "چراغ پس زمینه صفحه کلید") == 0) {
            if (strcmp(value, "دارد") == 0)
                keyboard->backround_light = 1;
            else
                keyboard->backround_light = 0;
            continue;
        }
        if (strcmp(key, "مقاوم در برابر گرد و غبار") == 0) {
            if (strcmp(value, "بله") == 0)
                keyboard->dustproof = 1;
            else
                keyboard->dustproof = 0;
            continue;
        }
    }
    fclose(stream);
    return 0;
}

int insert_keyboard(Keyboard *keyboard) {
    char insert_cmd[8000];
    sprintf(insert_cmd,
            "INSERT INTO keyboard (product_id, size, cable_type, os_compatible, weight, background_light, shortcut_key_no, mice_range, accuracy, fa_letter, keys_no, dustproof, power, cable_len, conn_type, usb_input) VALUE (%d, \"%s\", \"%s\", \"%s\", %d, %d, %d, \"%s\", \"%s\", %d, %d, %d, \"%s\", \"%s\", \"%s\", %d);",
            keyboard->product_id, keyboard->size, keyboard->cable_type, keyboard->os_compatible, keyboard->weight,
            keyboard->backround_light, keyboard->shortcut_key_no, keyboard->mice_range, keyboard->accuracy,
            keyboard->fa_letter, keyboard->keys_no,
            keyboard->dustproof, keyboard->power, keyboard->cable_len, keyboard->conn_type, keyboard->usb_input);
    if (mysql_query(mysql, insert_cmd) != 0) {
        printf("insert keyboard is: %s\n", insert_cmd);
        return -1;
    }
    return 0;
}

int extract_mice_attr(TempProduct *product, Mice *mice) {
    char token[550], attr[8000];
    strcpy(attr, product->product_attr);
    sscanf(attr, "[%[^]]", attr);
    FILE *stream = fmemopen(attr, strlen(attr), "r");
    mice->product_id = atoi(product->id);
    while (fscanf(stream, "{%[^}]},", token) != EOF) {
        char key[50] = {0}, value[500] = {0};
        if (strstr(token, "Value") == NULL)
            continue;
        sscanf(token, "\"Key\":\"%[^\"]\",\"Value\":\"%[^\"]\"", key, value);
        if (strcmp(key, "ابعاد") == 0) {
            strcpy(mice->size, value);
            continue;
        }
        if (strcmp(key, "نوع اتصال") == 0) {
            strcpy(mice->conn_type, value);
            continue;
        }
        if (strcmp(key, "وزن") == 0) {
            mice->weight = atoi(value);
            continue;
        }
        if (strcmp(key, "سازگار با سیستم‌عامل‌های") == 0) {
            strcpy(mice->os_compatible, value);
            continue;
        }
        if (strcmp(key, "طول کابل") == 0) {
            strcpy(mice->cable_len, value);
            continue;
        }
        if (strcmp(key, "نوع رابط") == 0) {
            strcpy(mice->cable_type, value);
            continue;
        }
        if (strcmp(key, "نوع حسگر") == 0) {
            strcpy(mice->sensor_type, value);
            continue;
        }
        if (strcmp(key, "محدوده دقت") == 0) {
            strcpy(mice->accuracy_range, value);
            continue;
        }
        if (strcmp(key, "قابلیت کارکردن با هر دو دست") == 0) {
            if (strcmp(value, "بله") == 0)
                mice->two_hand_used = 1;
            else
                mice->two_hand_used = 0;
            continue;
        }
    }
    fclose(stream);
    return 0;
}

int insert_mice(Mice *mice) {
    char insert_cmd[8000];
    sprintf(insert_cmd,
            "INSERT INTO mice (product_id, os_compatible, cable_len, size, sensor_type, conn_type, accuracy_range, weight, two_hand_used, cable_type) VALUE (%d, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %d, %d, \"%s\");",
            mice->product_id, mice->os_compatible, mice->cable_len, mice->size, mice->sensor_type, mice->conn_type,
            mice->accuracy_range, mice->weight, mice->two_hand_used, mice->cable_type);
    if (mysql_query(mysql, insert_cmd) != 0) {
        return -1;
    }
    return 0;
}

int extract_screen_attr(TempProduct *product, ScreenGuard *screen) {
    char token[1000], attr[10000];
    strcpy(attr, product->product_attr);
    sscanf(attr, "[%[^]]", attr);
    FILE *stream = fmemopen(attr, strlen(attr), "r");
    screen->product_id = atoi(product->id);
    while (fscanf(stream, "{%[^}]},", token) != EOF) {
        char key[100] = {0}, value[900] = {0};
        if (strstr(token, "Value") == NULL)
            continue;
        sscanf(token, "\"Key\":\"%[^\"]\",\"Value\":\"%[^\"]\"", key, value);
        if (strcmp(key, "نوع") == 0) {
            strcpy(screen->type, value);
            continue;
        }
        if (strcmp(key, "مناسب برای گوشی های") == 0) {
            strcpy(screen->compatible_with, value);
            continue;
        }
        if (strcmp(key, "قابلیت نصب آسان") == 0) {
            if (strcmp(value, "بله") == 0)
                screen->easy_to_use = 1;
            else
                screen->easy_to_use = 0;
            continue;
        }
        if (strcmp(key, "ضخامت") == 0) {
            strcpy(screen->thickness, value);
            continue;
        }
        if (strcmp(key, "جلوگیری از ایجاد خط و خش") == 0) {
            if (strcmp(value, "دارد") == 0)
                screen->scratches_prevent = 1;
            else
                screen->scratches_prevent = 0;
            continue;
        }
        if (strcmp(key, "مقاوم در برابر ضربه") == 0) {
            if (strcmp(value, "دارد") == 0)
                screen->strike_prevent = 1;
            else
                screen->strike_prevent = 0;
            continue;
        }
        if (strcmp(key, "جلوگیری از انعکاس نور") == 0) {
            if (strcmp(value, "دارد") == 0)
                screen->light_protecting = 1;
            else
                screen->light_protecting = 0;
            continue;
        }
        if (strcmp(key, "مشخصات دیگر") == 0) {
            strcpy(screen->other_attr, value);
            continue;
        }
    }
    fclose(stream);
    return 0;
}

int insert_screen(ScreenGuard *screen) {
    char insert_cmd[8000];
    sprintf(insert_cmd,
            "INSERT INTO screen_guard (product_id, type, compatible_with, easy_to_use, thickness, scratches_prevent, strike_prevent, light_protecting, other_attr) VALUE (%d, \"%s\", \"%s\", %d, %f, %d, %d, %d, \"%s\");",
            screen->product_id, screen->type, screen->compatible_with, screen->easy_to_use, atof(screen->thickness),
            screen->scratches_prevent,
            screen->strike_prevent, screen->light_protecting, screen->other_attr);
    if (mysql_query(mysql, insert_cmd) != 0) {
        printf("insert screen is: %s\n", insert_cmd);
        return -1;
    }
    return 0;
}

int extract_cover_attr(TempProduct *product, PhoneCover *cover) {
    char token[1000], attr[10000];
    strcpy(attr, product->product_attr);
    sscanf(attr, "[%[^]]", attr);
    FILE *stream = fmemopen(attr, strlen(attr), "r");
    cover->product_id = atoi(product->id);
    while (fscanf(stream, "{%[^}]},", token) != EOF) {
        char key[100] = {0}, value[900] = {0};
        if (strstr(token, "Value") == NULL)
            continue;
        sscanf(token, "\"Key\":\"%[^\"]\",\"Value\":\"%[^\"]\"", key, value);
        if (strcmp(key, "نوع") == 0) {
            strcpy(cover->type, value);
            continue;
        }
        if (strcmp(key, "مناسب برای گوشی موبایل") == 0) {
            strcpy(cover->compatible_with, value);
            continue;
        }
        if (strcmp(key, "وزن") == 0) {
            cover->weight = atoi(value);
            continue;
        }
        if (strcmp(key, "ابعاد") == 0) {
            strcpy(cover->size, value);
            continue;
        }
        if (strcmp(key, "ساختار") == 0) {
            strcpy(cover->structure, value);
            continue;
        }
        if (strcmp(key, "جنس") == 0) {
            strcpy(cover->material, value);
            continue;
        }
        if (strcmp(key, "قابلیت‌های ویژه") == 0) {
            strcpy(cover->special_attr, value);
            continue;
        }
        if (strcmp(key, "سطح پوشش") == 0) {
            strcpy(cover->cover_surface, value);
            continue;
        }
    }
    fclose(stream);
    return 0;
}

int insert_cover(PhoneCover *cover) {
    char insert_cmd[8000];
    sprintf(insert_cmd,
            "INSERT INTO phone_cover (product_id, compatible_with, weight, type, size, structure, material, special_attr, cover_surface) VALUE (%d, \"%s\", %d, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\");",
            cover->product_id, cover->compatible_with, cover->weight, cover->type, cover->size, cover->structure,
            cover->material, cover->special_attr, cover->cover_surface);
    if (mysql_query(mysql, insert_cmd) != 0) {
        printf("insert cover is: %s\n", insert_cmd);
        return -1;
    }
    return 0;
}

int load_products() {
    FILE *fp;
    fp = fopen(products_dataset_path, "r");
    if (fp == NULL)
        return -1;
    char line[10000];
    TempProduct *product_tmp = (TempProduct *) malloc(sizeof(TempProduct));
    while (fscanf(fp, "%[^\n]\n", line) != EOF) {
        parse_csv(line, product_tmp, "^");
        // Insert book
        if (strstr(product_tmp->category_title_fa, book_title_fa) != NULL && isdigit(line[0]) != 0) {
//            parse_csv(line, product_tmp, "^");
            int category_id = insert_category(product_tmp);
            int brand_id = insert_brand(product_tmp);
            if (category_id == -1 || brand_id == -1 || insert_product(product_tmp, category_id, brand_id) == -1) {
                printf("\tInsert product #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
                continue;
            }
            Book book = {0};
            extract_book_attr(product_tmp, &book);
            if (insert_book(&book) != 0)
                printf("\tInsert book #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
            continue;
        }

        // Insert puzzle
        if (strstr(product_tmp->category_title_fa, puzzle_title_fa) != NULL && isdigit(line[0]) != 0) {
//            parse_csv(line, product_tmp, "^");
            int category_id = insert_category(product_tmp);
            int brand_id = insert_brand(product_tmp);
            if (category_id == -1 || brand_id == -1 || insert_product(product_tmp, category_id, brand_id) == -1) {
                printf("\tInsert product #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
                continue;
            }
            Puzzle puzzle = {0};
            extract_puzzle_attr(product_tmp, &puzzle);
            if (insert_puzzle(&puzzle) != 0)
                printf("\tInsert puzzle #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
            continue;
        }

        // Insert keyboard
        if (strstr(product_tmp->category_title_fa, keyboard_title_fa) != NULL && isdigit(line[0]) != 0) {
//            parse_csv(line, product_tmp, "^");
            int category_id = insert_category(product_tmp);
            int brand_id = insert_brand(product_tmp);
            if (category_id == -1 || brand_id == -1 || insert_product(product_tmp, category_id, brand_id) == -1) {
                printf("\tInsert product #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
                continue;
            }
            Keyboard keyboard = {0};
            extract_keyboard_attr(product_tmp, &keyboard);
            if (insert_keyboard(&keyboard) != 0)
                printf("\tInsert keyboard #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
            continue;
        }

        // Insert mice
        if (strstr(product_tmp->category_title_fa, mice_title_fa) != NULL && isdigit(line[0]) != 0) {
//            parse_csv(line, product_tmp, "^");
            int category_id = insert_category(product_tmp);
            int brand_id = insert_brand(product_tmp);
            if (category_id == -1 || brand_id == -1 || insert_product(product_tmp, category_id, brand_id) == -1) {
                printf("\tInsert product #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
                continue;
            }
            Mice mice = {0};
            extract_mice_attr(product_tmp, &mice);
            if (insert_mice(&mice) != 0)
                printf("\tInsert mice #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
            continue;
        }

        // Insert screenguard
        if (strstr(product_tmp->category_title_fa, screen_title_fa) != NULL && isdigit(line[0]) != 0) {
//            parse_csv(line, product_tmp, "^");
            int category_id = insert_category(product_tmp);
            int brand_id = insert_brand(product_tmp);
            if (category_id == -1 || brand_id == -1 || insert_product(product_tmp, category_id, brand_id) == -1) {
                printf("\tInsert product #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
                continue;
            }
            ScreenGuard screen = {0};
            extract_screen_attr(product_tmp, &screen);
            if (insert_screen(&screen) != 0)
                printf("\tInsert screenguard #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
            continue;
        }

//        Insert phone cover
        if (strstr(product_tmp->category_title_fa, cover_title_fa) != NULL && isdigit(line[0]) != 0) {
//            parse_csv(line, product_tmp, "^");
            int category_id = insert_category(product_tmp);
            int brand_id = insert_brand(product_tmp);
            if (category_id == -1 || brand_id == -1 || insert_product(product_tmp, category_id, brand_id) == -1) {
                printf("\tInsert product #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
                continue;
            }
            PhoneCover cover = {0};
            extract_cover_attr(product_tmp, &cover);
            if (insert_cover(&cover) != 0)
                printf("\tInsert cover #%s failed: %s\n", product_tmp->id, mysql_error(mysql));
            continue;
        }
    }
    free(product_tmp);
    fclose(fp);
    return 0;
}
