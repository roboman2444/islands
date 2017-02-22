#include <stdio.h>
#include <stdlib.h>


#define STACK_DEPTH 100

static char *map[] = {	"10101010",
			"01110101",
			"00001100",
			"11100000",
			"11110110", 0};



char * data = 0;
int width, height;
char* convertMapToBase(char** map){
	//get dimensions
	int x, y, max = 0;
	for(y = 0; map[y]; y++){
		for(x=0; map[y][x]; x++);
		if(max < x) max = x;
	}
	width = max, height = y;

	if(data) free(data);
	data = malloc(width * height);

	for(y = 0; y < height; y++){
		for(x = 0; x < width && map[y][x]; x++)
			data[y*width + x] = map[y][x] == '0' ? 0 : 1;
		for(;x < width; x++)
			data[y*width + x] = 0;

	}
	return data;
}
void printBase(char * data, int width, int height){
	int x, y;
	for(y = 0; y < height; y++){
		for(x = 0; x < width; x++)
			putc(data[y*width+x] +'0', stdout);
		putc('\n', stdout);
	}
}




typedef struct simplestack_s {
	int x, y, plc;
} simplestack_t;

int count(char * data, int width, int height){
	int count = 0;

	simplestack_t stack[STACK_DEPTH];



	//check every
	int x, y;
	for(y = 0; y < height; y++){
		for(x = 0; x < width; x++){
			//havent already gone over this guy, and it is land
			if(data[y*width+x]){
				count++;
				data[y*width +x] = 0;
				//flood fill
				int stackdepth = 0;
				stack[stackdepth].x = x;
				stack[stackdepth].y = y;
				stack[stackdepth].plc = 0;
//				printf("checking %i, %i\n", x, y);
				do {
					int curx = stack[stackdepth].x;
					int cury = stack[stackdepth].y;
//					printf("stackdepth %i, curx %i, cury %i, plc %i\n", stackdepth, curx, cury, stack[stackdepth].plc);
					switch(stack[stackdepth].plc){
						case 0:
						if(cury>y && data[(cury-1) * width + curx]){ // up//gaurenteed to not have any "missed" islands vertically
							stack[stackdepth].plc = 1; //so when i return i move to the next check
							stackdepth++;
							stack[stackdepth].x = curx;
							stack[stackdepth].y = cury-1;
							stack[stackdepth].plc = 0;
							data[(cury-1) * width + curx] = 0;
							continue; //recursive call
						}
						case 1:
						if(cury+1 < height && data[(cury+1) * width + curx]){ //down
							stack[stackdepth].plc = 2;
							stackdepth++;
							stack[stackdepth].x = curx;
							stack[stackdepth].y = cury+1;
							stack[stackdepth].plc = 0;
							data[(cury+1) * width + curx] = 0;
							continue;
						}
						case 2:
						if(curx>0 && data[cury * width + curx-1]){ //left // not gaurenteed to have any missed islands horizontally
							stack[stackdepth].plc = 3;
							stackdepth++;
							stack[stackdepth].x = curx-1;
							stack[stackdepth].y = cury;
							stack[stackdepth].plc = 0;
							data[cury * width + curx-1] = 0;
							continue;
						}
						case 3:
						if(curx+1 < width && data[cury * width + curx+1]){ //right
							stack[stackdepth].plc=4;
							stackdepth++;
							stack[stackdepth].x = curx+1;
							stack[stackdepth].y = cury;
							stack[stackdepth].plc = 0;
							data[cury * width + curx+1] = 0;
							continue;
						}
						//checked everything, "return"
						default:
						stackdepth--;
					}
				}while(stackdepth>0);

			}
		}
	}
	return count;
}


int main(void){
	int i;
	convertMapToBase(map);
	printBase(data, width, height);
	int num = count(data, width, height);
	printf("num was %i\n", num);

	return 0;
}
