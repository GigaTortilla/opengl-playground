#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <unistd.h>
#include <stdbool.h>
#include <gl_test.h>

//void create_test_file(char** argv) {
//	char path[1000];
//	getcwd(path, sizeof(path));
//	strcat(path, "\\test.txt");
//
//	FILE* file = fopen(path, "w");
//	char* test_text = "Hello World!\nThis is a second line.";
//	fwrite(test_text, 1, strlen(test_text), file);
//	fclose(file);
//	
//	free(argv[1]);
//	argv[1] = malloc(strlen(path));
//	strcpy(argv[1], path);
//}

int main(int argc, char** argv) {
	//// create test file and pass into first argument if not specified
	//if (argc == 1)
	//	create_test_file(argv);
	//
	//// get file from first argument
	//FILE* file = fopen(argv[1], "r");
	//
	//// read file into buffer and print content
	//char contents[1024] = { 0 };
	//fread(contents, 1024, 1, file);
	//fclose(file);
	//printf("Contents:\n%s\n", contents);

	//render_image_mix();
	spin_and_scale();

	return 0;
}