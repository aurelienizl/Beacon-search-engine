#include "gui.h"

struct results
{
    char *title;
    char *url;
    char *description;
    char *block;
    struct results *next;
};

struct results *generate_random_results()
{
    struct results *results = NULL;
    struct results *current = NULL;

    for (int i = 0; i < 10; i++)
    {
        struct results *new_result = malloc(sizeof(struct results));
        // Generate random title.
        new_result->title = strdup("Titled");
        new_result->url = strdup("http://www.google.fr");
        new_result->description = strdup("Description");
        new_result->next = NULL;
        if (results == NULL)
        {
            results = new_result;
            current = results;
        }
        else
        {
            current->next = new_result;
            current = current->next;
        }
    }
    return results;
}

void free_results(struct results *results)
{
    struct results *current = results;
    while (current != NULL)
    {
        struct results *next = current->next;
        free(current->title);
        free(current->url);
        free(current->description);
        free(current->block);
        free(current);
        current = next;
    }
}

#pragma region Html_Builder

char* file_to_array(char* path)
{
    FILE* file = fopen(path, "rb");  // Open the file in binary mode
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file: %s\n", path);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0)  // Seek to the end of the file
    {
        fprintf(stderr, "Failed to seek file: %s\n", path);
        fclose(file);
        return NULL;
    }

    long file_size = ftell(file);  // Get the size of the file
    if (file_size == -1)
    {
        fprintf(stderr, "Failed to get the file size: %s\n", path);
        fclose(file);
        return NULL;
    }

    rewind(file);  // Seek back to the start of the file

    char* buffer = malloc(file_size + 1);
    if (buffer == NULL)
    {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    long read = fread(buffer, 1, file_size, file);  // Read the entire file into the buffer
    if (read != file_size)
    {
        fprintf(stderr, "Failed to read the file: %s\n", path);
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';

    fclose(file);

    return buffer;
}

char *build_block_result(const char *title, const char *url,const char *description) {
    const char *block = 
    "<div class=\"searchresult\">"
        "<h2>{TITLE}</h2>"
        "<a href=\"{URL}\">{URL}</a> <button>▼</button>"
        "<p>{DESCRIPTION}</p>"
    "</div>";

    size_t blockLength = strlen(block);
    size_t titleLength = strlen(title);
    size_t urlLength = strlen(url);
    size_t descriptionLength = strlen(description);

    char *res = malloc(blockLength + titleLength + urlLength + descriptionLength + 1);
    if (res == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    char *ptr = res;
    const char *start = block;
    while (*start) {
        switch(*start) {
            case '{':
                if (strncmp(start, "{TITLE}", 7) == 0) {
                    memcpy(ptr, title, titleLength);
                    ptr += titleLength;
                    start += 7;
                } else if (strncmp(start, "{URL}", 5) == 0) {
                    memcpy(ptr, url, urlLength);
                    ptr += urlLength;
                    start += 5;
                } else if (strncmp(start, "{DESCRIPTION}", 13) == 0) {
                    memcpy(ptr, description, descriptionLength);
                    ptr += descriptionLength;
                    start += 13;
                } else {
                    *ptr++ = *start++;
                }
                break;
            default:
                *ptr++ = *start++;
                break;
        }
    }
    *ptr = '\0';
    return res;
}

void replace_substring(char **str,char *to_replace, char *replace_with) {
    size_t len_str = strlen(*str);
    size_t len_to_replace = strlen(to_replace);
    size_t len_replace_with = strlen(replace_with);

    size_t count = 0;
    for (char *sub = *str; (sub = strstr(sub, to_replace)) != NULL; sub += len_to_replace) {
        ++count;
    }

    size_t len_new_str = len_str + count * (len_replace_with - len_to_replace);
    char *new_str = malloc(len_new_str + 1);
    if (!new_str) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    char *old_sub = *str, *new_sub = new_str;
    while (count--) {
        char *start = strstr(old_sub, to_replace);
        size_t len = start - old_sub;
        new_sub = stpncpy(new_sub, old_sub, len);
        new_sub = stpcpy(new_sub, replace_with);
        old_sub += len + len_to_replace;
    }
    strcpy(new_sub, old_sub);

    free(*str);
    *str = new_str;
}

void insert_blocks_into_template(char **template, struct results *results) {
    // The start and end placeholders.
    char *start_placeholder = "<p><!-- // TODO : Insert block code in this section --></p>";
    char *end_placeholder = "<p><!-- // The section ends here --></p>";

    struct results *current = results;

    // Insert all blocks into the template.
    while (current != NULL) {
        // Build the new block.
        char *new_block = malloc(strlen(start_placeholder) + strlen(current->block) + 1);
        if (new_block == NULL) {
            perror("Memory allocation failed");
            return;
        }
        strcpy(new_block, start_placeholder);
        strcat(new_block, current->block);

        // Replace the start placeholder with the new block in the template.
        replace_substring(template, start_placeholder, new_block);

        free(new_block);
        current = current->next;
    }

    // Now that all blocks have been inserted, replace the start placeholder with an empty string.
    replace_substring(template, start_placeholder, "");

    // Replace the end placeholder with the start placeholder, ready for the next set of blocks.
    replace_substring(template, end_placeholder, start_placeholder);
}

void array_to_file(char* path, char* data)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf("Failed to open file %s\n", path);
        return;
    }

    fprintf(fp, "%s", data);
    fclose(fp);
}

#pragma endregion Htlm_Builder

int launch_searcher(const gchar* request)
{
    struct results *results = generate_random_results();

    if (results == NULL)
    {
        fprintf(stderr, "Failed to generate results\n");
        return 1;
    }

    int results_number = 0;
    char str[12];
    size_t str_size = sizeof(str);  // Here is the new size_t variable

    struct results *current = results;
    while (current != NULL)
    {
        char *block = build_block_result(current->title, current->url, current->description);
        if (block == NULL)
        {
            fprintf(stderr, "Failed to build block result\n");
            free_results(results);
            return 1;
        }
        current->block = block;
        current = current->next;
        results_number++;
    }

    char *template = file_to_array("interface/results/index.html");
    char* ip = get_public_ip();

    if (template == NULL || ip == NULL)
    {
        fprintf(stderr, "Failed to load template or get public IP\n");
        free_results(results);
        free(ip);
        return 1;
    }
    
    int ret = snprintf(str, str_size, "%d", results_number);
    if(ret < 0 || (size_t)ret >= str_size)  // Compare with str_size, and cast ret to size_t
    {
        fprintf(stderr, "Failed to write result number to string\n");
        free_results(results);
        free(template);
        free(ip);
        return 1;
    }

    replace_substring(&template, "{RESEARCHSTR}", (char*) request);
    replace_substring(&template, "{IPLOCATION}", ip);
    replace_substring(&template, "{RESULTSNUMBER}", str);
    insert_blocks_into_template(&template, results);

    array_to_file("interface/results/generated_page.html", template);

    // Open the generated page in the default browser.
    int status = system("xdg-open interface/results/generated_page.html");
    if (status == -1)
    {
        fprintf(stderr, "Failed to open generated page in the default browser\n");
        free_results(results);
        free(template);
        free(ip);
        return 1;
    }
    // Cleanup
    
    free_results(results);
    free(template);
    free(ip);

    return 0;
}

