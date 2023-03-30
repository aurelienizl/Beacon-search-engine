word_info_t *extract_words(xmlDocPtr doc) 
{
    word_info_t *word_infos = NULL;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        fprintf(stderr, "Error getting root element\n");
        return NULL;
    }
    xmlNodePtr cur = root;
    while (cur != NULL) {
        if (cur->type == XML_TEXT_NODE && cur->parent != NULL && cur->parent->type == XML_ELEMENT_NODE) {
            // Extract the words from the text node
            char *text = (char *)cur->content;
            char *word = strtok(text, " ,.;:()[]{}<>'\"\n\r\t");
            while (word != NULL) {
                // Convert the word to lowercase
                for (int i = 0; word[i]; i++) {
                    word[i] = tolower(word[i]);
                }
                // Add the word to the word_info linked list
                word_info_t *wi = find_word_info(word_infos, word);
                if (wi == NULL) {
                    wi = (word_info_t *)malloc(sizeof(word_info_t));
                    wi->word = strdup(word);
                    wi->count = 1;
                    wi->num_positions = 1;
                    wi->positions = (int *)malloc(sizeof(int));
                    wi->positions[0] = cur->parent->line;
                    wi->next = word_infos;
                    word_infos = wi;
                } else {
                    wi->count++;
                    wi->num_positions++;
                    wi->positions = (int *)realloc(wi->positions, wi->num_positions * sizeof(int));
                    wi->positions[wi->num_positions - 1] = cur->parent->line;
                }
                word = strtok(NULL, " ,.;:()[]{}<>'\"\n\r\t");
            }
        }
        cur = cur->xmlChildrenNode;
    }
    return word_infos;
}

void free_word_info(word_info_t *word_infos) 
{
    while (word_infos != NULL) {
        word_info_t *next = word_infos->next;
        free(word_infos->word);
        free(word_infos->positions);
        free(word_infos);
        word_infos = next;
    }
}

int save_word_info(sqlite3* db, word_info_t* word_info, int num_words, char *document_id) 
{
    sqlite3_stmt* stmt;
    int rc;
    char filename[256];
    sprintf(filename, "../barrels/%s.db", document_id);
    // Open the database file
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    // Prepare the SQL statement for inserting word info into the database
    const char* sql = "INSERT INTO word_info (word, count, positions) VALUES (?, ?, ?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    // Bind each word_info to the statement parameters and execute the statement
    for (int i = 0; i < num_words; i++) {
        word_info_t* wi = &word_info[i];
        // Create a comma-separated string of positions for this word
        char positions_str[MAX_POSITIONS_STR_LEN] = "";
        for (int j = 0; j < wi->num_positions; j++) {
            char pos_str[10];
            sprintf(pos_str, "%d", wi->positions[j]);
            if (j != 0) {
                strcat(positions_str, ",");
            }
            strcat(positions_str, pos_str);
        }
        // Bind the values to the statement parameters and execute the statement
        sqlite3_bind_text(stmt, 1, wi->word, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, wi->count);
        sqlite3_bind_text(stmt, 3, positions_str, -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) 
        {
            fprintf(stderr, "Error inserting word info: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
    // Close the database connection
    sqlite3_close(db);
    free_word_info(word_info);
    return 0;
}


int build_barrels(char *content, char *document_id) 
{
    xmlDocPtr doc;
    doc = htmlReadMemory(content, strlen(content), NULL, NULL, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
    if (doc == NULL) 
    {
        fprintf(stderr, "Error parsing document\n");
        return 1;
    }
    // Extract the words from the HTML document
    word_info_t *word_infos = extract_words(doc);
    // Save the word info to the database
    int rc = save_word_info(word_infos, document_id);
    if (rc != 0) 
    {
        fprintf(stderr, "Error saving word info\n");
        return 1;
    }
    return 0;
}