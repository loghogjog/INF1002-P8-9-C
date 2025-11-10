#include <headers.h>

//write or save file
int save(char *filename, char *data) {
	//save to file in writable mode
	FILE *file = fwrite(FILENAME, "w");
	
	//throw error if file fail to save
	if (file == NULL) {
		printf("Fail to save file");
		return -1;
	}
	//save whatever into txt file via fprintf, close the file
	fprintf(file, "%s", data);
	fclose(file);
	return 0;
}
	
		
	