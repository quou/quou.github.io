#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#define OK 0;
#define ERROR_INCORRECT_ARGS -1;
#define FILE_ERROR -2;

static const char* html_begin = "<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"<meta charset=\"UTF-8\"></meta>\n"
"<link rel=\"stylesheet\" href=\"style.css\"></link>\n"
"<title>George Lamb</title>\n"
"</head>\n"
"<body>\n"
"<div class=\"content\">\n"
"<p><a href=\"index.html\"><-Home Page</a></p>";

static const char* html_end = "</div>\n</body>\n"
"</html>\n";

void parse_file_location(char* file, char* path);
char* read_text(FILE* file);
char* render_markdown(const char* markdown);

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("usage: plce [sitefile] [out_dir]\n");
		return ERROR_INCORRECT_ARGS;
	}

	/* Get the working directory */
	char wrk_dir[256];
	if (strchr(argv[1], '/') != NULL) {
		parse_file_location(argv[1], wrk_dir);
	}

	/* Create the output directories */
	struct stat st = {0};
	if (stat(argv[2], &st) == -1) {
	    mkdir(argv[2], 0700);
	}

	/* Open the sitefile */
	FILE* sitefile_handle = fopen(argv[1], "r");
	if (sitefile_handle == NULL) {
		printf("Cannot open %s\n", argv[1]);
		return FILE_ERROR;
	}

	char line[512];
	/* Loop over lines in the sitefile */
	while (fgets(line, sizeof(line), sitefile_handle)) {
		char* md_buffer = strtok(line, " ");
		char* html_buffer = strtok(NULL, " ");

		char md_path[512] = { '\0' };
		sprintf(md_path, "%s/%s", wrk_dir, md_buffer);

		char html_path[512] = { '\0' };
		sprintf(html_path, "%s/%s", argv[2], html_buffer);

		const int html_path_len = strlen(html_path);
		if (html_path[html_path_len - 1] == '\n') {
			html_path[html_path_len - 1] = '\0';
		}

		printf("Generating `%s' using `%s'...", html_path, md_path);

		FILE* md_handle = fopen(md_path, "r");
		char* md_text = read_text(md_handle);
		if (md_text == NULL) {
			printf("\nCannot find %s\n", md_path);
			return FILE_ERROR;
		}
		fclose(md_handle);

		char* gen_html = render_markdown(md_text);
		if (!gen_html) {
			free(md_text);
			printf(" Cancelled\n");
			continue;
		}

		FILE* html_handle = fopen(html_path, "w");
		if (html_handle == NULL) {
			printf("\nCannot find %s\n", html_path);
			return FILE_ERROR;
		}
		fputs(gen_html, html_handle);
		fclose(html_handle);

		free(md_text);
		free(gen_html);

		printf(" Done\n");
    }

    fclose(sitefile_handle);

	return OK;
}

void parse_file_location(char* file, char* path) {
	const int len = strlen(file);

	char* last = file + len;
	while (*last != '/' && last >= file) { last--; }

	strncpy(path, file, last-file);
	path[last-file] = '\0';
}

static void parse_paragraph(char* line, char* out) {
	const int len = strlen(line);

	bool in_code = false;

	char* c = line;
	while (*c != '\0') {
		char str[1024] = { '\0' };

		if (*c == '[') { /* Parse links */
			char link_name[256] = { '\0' };
			char link_addr[256] = { '\0' };
			char link_html[2048] = { '\0' };

			c++;
			while (*c != ']') {
				strncat(link_name, c, 1);

				c++;
			}
			c++;

			if (*c == '(') {
				c++;
				while (*c != ')') {
					strncat(link_addr, c, 1);

					c++;
				}
			}
			sprintf(link_html, "<a class=\"link\" href=\"%s\">%s</a>", link_addr, link_name);
			strcat(str, link_html);
		} else if (*c == '!' && *(c + 1) == '[') { /* Parse images */
			c++;
			char link_name[256] = { '\0' };
			char link_addr[256] = { '\0' };
			char link_html[1024] = { '\0' };

			c++;
			while (*c != ']') {
				strncat(link_name, c, 1);

				c++;
			}
			c++;

			if (*c == '(') {
				c++;
				while (*c != ')') {
					strncat(link_addr, c, 1);

					c++;
				}
			}
			sprintf(link_html, "<img class=\"image\" src=\"%s\" alt=\"%s\"></img>", link_addr, link_name);
			strcat(str, link_html);
		} else if (*c == '*') { /* Parse bold/italic text */
			c++;

			char tag = 'i';

			if (*c == '*') {
				tag = 'b';
				c++;
			}

			char styled_text[1000] = { '\0' };
			char styled_html[1024] = { '\0' };

			while (*c != '*') {
				strncat(styled_text, c, 1);

				c++;
			}

			if (*c == '*') {
				c++;
			}

			sprintf(styled_html, "<%c>%s</%c>", tag, styled_text, tag);
			strcat(str, styled_html);
		} else if (*c == '`') {
			if (in_code) {
				strcat(str, "</code>");

				in_code = false;
			} else {
				strcat(str, "<code>");
				in_code = true;
			}
		} else {
			str[0] = *c;
			str[1] = '\0';
		}

		strcat(out, str);

		c++;
	}
}

char* render_markdown(const char* markdown) {
	const int len = strlen(markdown);
	if (len <= 10) {
		return NULL;
	}

	char* markdown_buffer = malloc(len * sizeof(char));
	strcpy(markdown_buffer, markdown);

	char* html_buffer = malloc((len * sizeof(char) * 2) + strlen(html_begin) + strlen(html_end));
	html_buffer[0] = '\0';

	strcat(html_buffer, html_begin);

	bool list_open = false;

	char previous_line[1024] = { '\0' };

	/* Loop over the lines in the markdown */
	char* line = markdown_buffer;
	while (line != NULL) {
		char* next = strchr(line, '\n');
		if (next) { *next = '\0'; }

		char line_buffer[2048] = { '\0' };

		char* mod_line = line;

		if (previous_line[1] == '-' && line[1] != '-') {
			strcat(line_buffer, "</ul>");
			list_open = false;
		}

		if (line[0] == '#') {
			int head_count = 1;
			if (line[1] == '#') { head_count++; }
			if (line[2] == '#') { head_count++; }
			if (line[3] == '#') { head_count++; }

			mod_line += head_count;
			if (line[head_count] == ' ') {
				mod_line++;
			}

			sprintf(line_buffer, "<h%d>%s</h%d>\n", head_count, mod_line, head_count);
		} else if (line[0] == ' ' && line[1] == '-' && line[2] == ' ') {
			mod_line += 3;

			if (!list_open) {
				strcat(line_buffer, "<ul>");
				list_open = true;
			}

			strcat(line_buffer, "<li>");
			parse_paragraph(mod_line, line_buffer);
			strcat(line_buffer, "</li>\n");
		} else if (line[0] != '\0') {
			strcat(line_buffer, "<p>");
			parse_paragraph(line, line_buffer);
			strcat(line_buffer, "</p>\n");
		}

		if (strlen(line_buffer) > 0 || line_buffer[0] == ' ') {
			strcat(html_buffer, line_buffer);
		}

		strcpy(previous_line, line);

		if (next) { *next = '\n'; }
		line = next ? (next + 1) : NULL;
	}

	strcat(html_buffer, html_end);

	free(markdown_buffer);

	return html_buffer;
}

char* read_text(FILE* file) {
	if (file == NULL) { return NULL; }

	fseek(file, 0L, SEEK_END);
	unsigned int file_size = ftell(file);
	rewind(file);

	char* buffer = malloc(file_size + 1);

	unsigned int bytes_read = fread(buffer, sizeof(char), file_size, file);

	buffer[bytes_read] = '\0';

	return buffer;
}
