#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct activity {
	char name[30];
	short int day[7];
	short int start;
	short int end;
	short int ltag;
	short int rtag;
	short int x;
	short int y;
	char comment[30];
	char venue[15];
	struct activity *next;
}activity_t;

void stringClear(char array[], int size);
int getDay(char array[]);
void terminate(FILE *fo);
void printConflict(FILE *fo);
void conflictingSub(FILE *fo, char sub1[], char sub2[], int i);
char getLetter(int time);

int main(){
	FILE *fp, *fo;
	activity_t *act, *curr, *head, *rover, *curr2, *head2;
	char temp[100], reader[10], dayTemp[2], timeTemp[4];
	double counter = 0;
	int i=0, j = 0, earliest = 2401, latest = 0, conflict = 0, isPrinted = 0, m =0, n = 0;
	
	//FILE OPENING
	fp = fopen("DemoCase5.txt", "r");
	fo = fopen("output5.svg", "w");
	rewind(fp);
	
	fprintf(fo, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	fprintf(fo, "\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg11.dtd\">");
	fprintf(fo, "\n<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"1375\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" height=\"1500\" overflow=\"scroll\" onload=\"init(evt)\">\n\n");
	
	
	head = NULL;
	curr = NULL;
	act = NULL;
	
	
	while(!feof(fp)){
		act = (activity_t*)malloc(sizeof(activity_t));
		counter++;
		//printf("%d ", counter);
		if(counter==1) {
			head = act;
		}
		else {
			curr->next = act;
		}
		curr = act;
		//copy the days
		stringClear(reader, 10); i=0;
		fgets(temp, 100, fp);
		j=0;
		while(temp[i] != '\0'){
			if(temp[i] == ';')j++;
			i++;
		}
		if(j != 4){printf("Input error. 5 fields are expected. DAY;TIME;REMARK;NAME;VENUE."); fprintf(fo, "</svg>");exit(1);}
		i=0;j=0;
		while(temp[i] != ';') {
			reader[i] = temp[i];
			i++;	
		}
		reader[i] = '\0';
		
		for(i=0; i<7; i++) act->day[i] = 0;
		if(reader[0] == '\0'){printf("Input error. No input for: DAY field."); fprintf(fo, "</svg>");exit(1);}
		for(i=0;i<strlen(reader);i++){
			if(reader[i] != 'S' && reader[i] != 'M' && reader[i] != 'T' && reader[i] != 'W' && reader[i] != 'h' && reader[i] != 'F' && reader[i] != 'a' && reader[i] !='u' ){
				printf("Input error. Invalid input for: DAY field."); fprintf(fo, "</svg>");exit(1);}
		}i=0;
		if(reader[1] == '\0' || (reader[2] == '\0' && reader[1] >= 'a')){
			//meaning there is only one day
			//therefore get ALL the info
			act->day[getDay(reader)] = 1;
			
			
		}
		else{
		//multiple days
			i=0;
			while(reader[i] != '\0'){
				if(reader[i+1] >= 'a'){
					//the day consists of two letters
					strncpy(dayTemp, &reader[i], 2);
					dayTemp[2] = '\0';
					act->day[getDay(dayTemp)] = 1;
					i++; i++;
					
				}
				else {
					//the day consists of one letter
					strncpy(dayTemp, &reader[i], 1);
					dayTemp[1] = '\0';
					act->day[getDay(dayTemp)] = 1;
					i++;
						
				}		
			}	
		}
		
		//start
		stringClear(reader, 10);
		i=0;
		while(temp[i] != ';') i++;
		i++;j=0;
		while(temp[i] != ';') {
			reader[j] = temp[i];
			i++; j++;
		}
		reader[j] = '\0'; i=0; j=0;
		if(reader[0] == '\0'){printf("Input error. No input for: TIME field."); fprintf(fo, "</svg>");exit(1);}
		for(i=0;i<strlen(reader);i++){
			if(reader[i] <='0' && reader[i] >= '9' && reader[i] != '-' && reader[i] != 'a' && reader[i] != 'p' && reader[i] != 'm' && reader[i] != 'n' ){
				printf("Input error. Invalid input for: TIME field."); fprintf(fo, "</svg>");exit(1);}
		}
		i=0;
		
		while(reader[i] != 'a' && reader[i] != 'p' && reader[i] != 'm' && reader[i] != 'n') {
			if(reader[i] >= '0' && reader[i] <= '9') {
				timeTemp[j] = reader[i];	
				j++;
			}
			i++;
		}
		timeTemp[j] = '\0';
		act->start = atoi(timeTemp);
		if((act->start/100 == 12 && reader[i] == 'a') || reader[i] == 'm') act->start = act->start - 1200;
		if((reader[i] == 'p') && act->start/100 != 12) act->start = act->start + 1200;
		
		
		
		//end
		j=0; i++;
		while(reader[i] != 'a' && reader[i] != 'p' && reader[i] != 'm' && reader[i] != 'n') {
			if(reader[i] >= '0' && reader[i] <= '9') {
				timeTemp[j] = reader[i];	
				j++;
			}
			i++;
		}
		timeTemp[j] = '\0';
		act->end = atoi(timeTemp);
		if(act->end/100 == 12 && reader[i] == 'a') act->end = act->end - 1200;
		if((reader[i] == 'p' && act->end/100 != 12) || reader[i] == 'm') act->end = act->end + 1200;
		
		
		

		//remark

		stringClear(reader, 10); j= 0; i=0;
		while(temp[i] != ';') i++; i++;
		while(temp[i] != ';') i++; i++;
		while(temp[i] != ';') {
			reader[j] = temp[i];
			i++; j++;
		}
		reader[j] = '\0';
		strcpy(act->comment, reader);
		
		//name
		if(reader[0] == '\0'){printf("Input error. No input for: NAME field"); fprintf(fo, "</svg>");exit(1);}

	 	i++; j=0;
		while(temp[i] != ';') {
			act->name[j] = temp[i];
			i++; j++;
		}
		act->name[j] = '\0';
		
		//place
		i++; j=0;
		while(temp[i] != '\0' && temp[i] != '\n') {
			act->venue[j] = temp[i];
			i++; j++;
		}
		act->venue[j] = '\0';
		
		//overnight
		rover = curr;
		for(i=0; i<7; i++){
			if(rover->start > rover->end ) {
				if(rover->day[i] == 1) {
					act = (activity_t*)malloc(sizeof(activity_t));

					act->day[(i+1)%7] = 1;
					act->start = 0;
					act->end = curr->end;
					strcpy(act->name,curr->name);
					strcpy(act->comment,curr->comment);
					strcpy(act->venue,curr->venue);
				
					curr->next = act;
					curr = act;
				}
				
				if(i==6)rover->end = 2400;
			}
		}

		

	}
	
	//separate activities by day
	head2 = (activity_t*)malloc(sizeof(activity_t));
	curr2 = head2;
	rover= head;
	while(rover!= NULL){
		for(j=0;j<7;j++){
				if(rover->day[j] == 1){
				act = (activity_t*)malloc(sizeof(activity_t));

				act->end = rover->end;
				act->start = rover->start;
				strcpy(act->name, rover->name);
				strcpy(act->comment,rover->comment);
				strcpy(act->venue,rover->venue);
				act->day[j] = 1;
				curr2->next = act;
				curr2 = act;
				}
		}
	rover = rover->next;	
	}
	
	rover = head;
	while(rover != NULL){
		curr= rover;
		free(curr);	
		rover=rover->next;
	}
	
	head = head2->next;
	curr=curr2;
	
	
	//CHECK CONFLICTS
	act = head;
	while(act != curr->next){
	rover = head;
	while(rover != curr->next){
		if((((act->start > rover->start && act->start < rover->end) || (rover->start > act->start && rover->start < act->end) || (act->end > rover->start && act->end < rover->end) || (rover->end > act->start && rover->end < act->end)) || (rover->start == act->start && rover->end == act->end)) && (rover != act)) {		
			for(i=0; i<7; i++){
			if((act->day[i] ==1 && rover->day[i] ==1) && (strcmp(act->name, rover->name) ==0)){
				if(rover->start < act->start && rover->end > act->end ){
					act->start = rover->start;
					act->end = rover->end;	
				}	
				else if(rover->start < act->start && rover->end < act->end){
					act->start = rover->start;	
					rover->end = act->end;
				}
				else if(rover->start > act->start && rover->end > act->end){
					rover->start = act->start;
					act->end = rover->end;	
				}
				else if(rover->start > act->start && rover->end < act->end){
					rover->start = act->start;
					rover->end = act->end;	
				}
				
			}
			}
		}
		rover = rover->next;
		}	act = act->next;
	}
	
	act = head;
	while(act != curr->next){
	rover = head;
	while(rover != curr->next){
		if((((act->start > rover->start && act->start < rover->end) || (rover->start > act->start && rover->start < act->end) || (act->end > rover->start && act->end < rover->end) || (rover->end > act->start && rover->end < act->end)) || (rover->start == act->start && rover->end == act->end)) && (rover != act)) {		
			for(i=0; i<7; i++){		
			if((act->day[(i+1)] == 1 && rover->day[i] == 1) && (strcmp(act->name, rover->name) == 0)){
				rover->rtag = 1; act->ltag =1;
				if(act->start<=rover->start && act->end >=rover->end) act->ltag = -1;
				else if(act->start>=rover->start && act->end <=rover->end) rover->rtag=-1;
				else if(act->start<=rover->start && act->end <=rover->end){
					rover->rtag = 99;
					act->ltag =99;
					act->y = act->end; act->x = rover->start;
					rover->y = act->end; rover->x = rover->start;		
				}
				else if(act->start>=rover->start && act->end >=rover->end){
					rover->rtag = 99;
					act->ltag = 99;
					act->y = rover->end; act->x = act->start;
					rover->y = rover->end; rover->x = act->start;
				}
			}
			
			if((act->day[i] == 1) && (rover->day[i] == 1) && (strcmp(act->name, rover->name) !=0)){
					conflict = 1;
					if(isPrinted == 0){
					printConflict(fo); isPrinted = 1;
					}
					if(strcmp(act->comment, "CONFLICT printed") != 0 || strcmp(rover->comment, "CONFLICT printed") != 0){
					conflictingSub(fo, act->name, rover->name, i);
					strcpy(act->comment, "CONFLICT printed");
					strcpy(rover->comment, "CONFLICT printed");
					}
				}
			}
			
		}
		i=0;
		for(i=0; i<7; i++){
		if((act->day[i] == 1) && (rover->day[i] == 1) && (strcmp(act->name, rover->name) ==0)){
			if(act->end == rover->start){
				rover->start = act->start;
				act->end = rover->end;	
			}	
			else if(rover->end == act->start){
				rover->end = act->end;
				act->start = rover->start;	
			}
		}}
		rover = rover->next;
		}	act = act->next;
	}
	if(conflict == 1) terminate(fo);


	//Get latest and earliest time
	rover = head;
	while(rover!=NULL) {
		if(rover->start < earliest) earliest = rover->start;
		rover = rover->next;	
	}
	rover = head;
	while(rover!=NULL) {
		if(rover->end > latest) latest = rover->end;
		rover = rover->next;	
	}
	
	//Making the sched
	if(latest%100 != 0) latest = latest+100;
	j=(latest-earliest)/100;
	j=((latest-earliest)-j*100) + j*60;
	if(latest%100 < earliest%100) j = j-40;
	j = (latest/100 - earliest/100) * 60;
	fprintf(fo, "<rect x=\"151\" y=\"1\" width=\"1050\" height=\"%f\" fill=\"powderblue\" fill-opacity=\"0.7\" stroke=\"skyblue\" />\n", j/1.5 + 55);
	fprintf(fo, "<rect x=\"151\" y=\"1\" width=\"1050\" height=\"55\" fill=\"steelblue\" stroke=\"skyblue\" />\n");
	fprintf(fo, "<rect x=\"1\" y=\"56\" width=\"150\" height=\"%f\" fill=\"steelblue\" stroke=\"skyblue\" />\n", j/1.5);
	counter = 55 + 40;
			while(counter != (j/1.5) + 55){
			fprintf(fo, "<line x1=\"1\" y1=\"%f\" x2=\"1201\" y2=\"%f\" stroke=\"white\" />\n", counter, counter);
			counter = counter + 40;
			}
	fprintf(fo, "<line x1=\"301\" y1=\"0\" x2=\"301\" y2=\"%f\" stroke=\"white\" />\n", j/1.5 + 55);
	fprintf(fo, "<line x1=\"451\" y1=\"0\" x2=\"451\" y2=\"%f\" stroke=\"white\" />\n", j/1.5 + 55);
	fprintf(fo, "<line x1=\"601\" y1=\"0\" x2=\"601\" y2=\"%f\" stroke=\"white\" />\n", j/1.5 + 55);
	fprintf(fo, "<line x1=\"751\" y1=\"0\" x2=\"751\" y2=\"%f\" stroke=\"white\" />\n", j/1.5 + 55);
	fprintf(fo, "<line x1=\"901\" y1=\"0\" x2=\"901\" y2=\"%f\" stroke=\"white\" />\n", j/1.5 + 55);
	fprintf(fo, "<line x1=\"1051\" y1=\"0\" x2=\"1051\" y2=\"%f\" stroke=\"white\" />\n\n", j/1.5 + 55);
	fprintf(fo, "<text x=\"1450\" y=\"55\">\n<tspan font-family=\"Lucida Sans Unicode\" fill=\"white\" font-weight=\"bold\">\n<tspan x=\"155\" y=\"45\" font-size=\"20\">Sunday</tspan>\n<tspan x=\"305\" y=\"45\" font-size=\"20\">Monday</tspan><tspan x=\"455\" y=\"45\" font-size=\"20\">Tuesday</tspan>\n<tspan x=\"605\" y=\"45\" font-size=\"20\">Wednesday</tspan>\n<tspan x=\"755\" y=\"45\" font-size=\"20\">Thursday</tspan>\n<tspan x=\"905\" y=\"45\" font-size=\"20\">Friday</tspan>\n<tspan x=\"1055\" y=\"45\" font-size=\"20\">Saturday</tspan>\n</tspan>\n</text>\n\n");
	
	//Printing the time
	i=1;
	m = (earliest/100)%12;
	if(m == 0) m =12;
	n = (earliest/100 + 1)%12;
	if(n==0) n = 12;
	fprintf(fo, "<text x=\"150\" y=\"%f\" style=\"text-anchor:end\">\n<tspan font-family=\"Lucida Sans Unicode\" fill=\"white\" font-weight=\"bold\">\n<tspan x=\"145\" y=\"90\" font-size=\"11\">%d.00%c - %d.00%c</tspan>\n", j/1.5, m, getLetter(earliest), n, getLetter((earliest / 100 +1) * 100));
	
	while(i <= (latest - earliest)/100){
		m = (earliest/100+i)%12; if(m==0) m = 12;
		n = (earliest/100 + i + 1)%12; if(n==0) n = 12;
		fprintf(fo, "<tspan x=\"145\" dy=\"40\" font-size=\"11\">%d.00%c  -  %d.00%c</tspan>\n", m, getLetter((earliest/100+i)*100), n, getLetter((earliest/100 + i + 1)*100) );
		i++;
	}

	fprintf(fo, "</tspan>
</text>\n\n");

	//Make activity rectangles
	rover = head;m=0;
	while(rover != NULL){
	for(i=0; i<7; i++){
		j = (rover->end - rover->start) / 100;
		j=((rover->end-rover->start)-j*100) + j*60;
		if(rover->end%100 < rover->start%100) j = j-40;
		if(rover->day[i] == 1){
			fprintf(fo, "\n\n<rect id=\"task%d\" x=\"%d\" y=\"%f\" width=\"150\" height=\"%f\" fill=\"ghostwhite\" stroke=\"skyblue\" stroke-width=\"2\"></rect>\n", m, (i+1)*150+1, 40*(rover->start/100 - earliest/100) + 55 + (rover->start%100)/1.5, j/1.5);
			//if((rover->ltag == 0 && rover->rtag != 0) || (rover->ltag != 0 && rover->rtag == 0) ||(rover->rtag == 0 && rover->ltag == 0)){
			fprintf(fo, "<text x=\"1050\" y=\"2000\" style=\"text-anchor:middle\">\n<tspan font-family=\"Lucida Sans Unicode\" fill=\"indigo\" >\n<tspan x=\"%d\" y=\"%f\" font-size=\"11\" >%s<set attributeName=\"font-size\" from=\"11\" to=\"18\" begin=\"mouseover\" end=\"mouseout\"/></tspan>\n</tspan></text>\n\n" ,(i+1)*150 +76, 40*(rover->start/100 - earliest/100) + 55 + (rover->start%100)/1.5 + (j/3 + 4), rover->name);
//	}
	}}
	rover = rover->next;m++;
}	

	//Remove divider of same adjacent activities
	rover = head;
	while(rover !=NULL){
		for(i=0;i<7; i++){
			if(rover->day[i] ==1){
				if(rover->rtag > 98){
				fprintf(fo, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke=\"ghostwhite\" stroke-width=\"5\" />\n", (i+2)*150,40*(rover->x/100 - earliest/100) + 56 + (rover->x%100)/1.5, (i+2)*150, 40*(rover->y/100 - earliest/100) + 54 + (rover->y%100)/1.5 );	
				}
				if(rover->rtag == 1){
				fprintf(fo, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke=\"ghostwhite\" stroke-width=\"5\" />\n", (i+2)*150,40*(rover->start/100 - earliest/100) + 56 + (rover->start%100)/1.5, (i+2)*150, 40*(rover->end/100 - earliest/100) + 54 + (rover->end%100)/1.5 );
				}	
				if(rover->ltag == 1)	fprintf(fo, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke=\"ghostwhite\" stroke-width=\"5\" />\n", (i+1)*150,40*(rover->start/100 - earliest/100) + 56 + (rover->start%100)/1.5, (i+1)*150, 40*(rover->end/100 - earliest/100) + 54 + (rover->end%100)/1.5 );
							
			}
			
		}
	rover = rover->next;	
	}
	
	//Popup
	rover = head; m= 0;
	while(rover!=NULL){
		for(i=0; i<7; i++){	
			if(rover->day[i] == 1){
			fprintf(fo, "<rect width=\"200\" height=\"60\" x=\"%d\" y=\"%f\" fill=\"greenyellow\" opacity=\"0.75\" visibility=\"hidden\" stroke=\"black\">\n", (i+1)*150+101, 40*(rover->end/100 - earliest/100) + 55 + (rover->end%100)/1.5 - 20);
			fprintf(fo, "<set attributeName=\"visibility\" from=\"hidden\" to=\"visible\" begin=\"task%d.click\" end=\"task%d.click\"/>\n</rect>\n\n", m, m);

			fprintf(fo, "<text id=\"thepopup\" font-family=\"Lucida Sans Unicode\" font-size=\"10\" x=\"%d\" y=\"%f\" fill=\"darkcyan\" visibility=\"hidden\"><tspan dx=\"0\" dy=\"0\">Name of activity: %s</tspan><tspan x=\"%d\" dy=\"20\">Remarks: %s</tspan><tspan x=\"%d\" dy=\"20\">Venue: %s</tspan>\n", (i+1)*150+106, 40*(rover->end/100 - earliest/100) + 55 + (rover->end%100)/1.5 -5 , rover->name, (i+1)*150+106,rover->comment, (i+1)*150+106,rover->venue);
			fprintf(fo, "<set attributeName=\"visibility\" from=\"hidden\" to=\"visible\" begin=\"task%d.click\" end=\"task%d.click\"/>\n</text>\n\n", m, m);
			}
		
		}
		m++;
		rover=rover->next;
	}
	

	fprintf(fo, "</svg>");
	
	//last thing to do!
	/*rover=head;
	while(rover!=NULL) {
		curr = rover;
		rover = rover->next;	
		free(curr);
	}	
	free(head);
	free(head2);
	free(curr);
	free(curr2);
	free(rover);*/
	fclose(fp);
	fclose(fo);
	printf("Execution done.");
return 0;}	

void stringClear(char array[], int size){
	int i;
	for(i=0; i<size; i++){
		array[i] = '\0';
	}	
}

int getDay(char array[]){
		int day = 0;
		if(strcmp(array, "Su") == 0) day = 0;
		else if(strcmp(array, "M") == 0) day = 1;
		else if(strcmp(array, "T") == 0) day = 2;
		else if(strcmp(array, "W") == 0) day = 3;
		else if(strcmp(array, "Th") == 0) day = 4;
		else if(strcmp(array, "F") == 0) day = 5;
		else if(strcmp(array, "Sa") == 0) day = 6;
		return day;
}	
void terminate(FILE *fo){
		fprintf(fo, "</tspan>\n");
		fprintf(fo, "</text>\n");
		fprintf(fo, "</svg>");
		printf("Execution done.");
		exit(1);
}

void printConflict(FILE *fo){
	fprintf(fo, "<rect width=\"2000\" height=\"1800\" fill=\"steelblue\" />\n<text x=\"100\" y=\"150\">\n<tspan font-family=\"Arial\" fill=\"white\">\n<tspan x=\"300\" dy=\"80\" font-size=\"40\">Sorry, the schedule cannot be displayed.</tspan>\n<tspan x=\"300\" dy=\"50\" font-size=\"30\">Conflicting activities:</tspan>");
}


void conflictingSub(FILE *fo, char sub1[], char sub2[], int i) {

	fprintf(fo, "<tspan x=\"370\" dy=\"50\" font-size=\"25\">%s and %s\t", sub1, sub2);
	if(i==0) fprintf(fo, "(Sunday)</tspan>\n");
	else if(i==1) fprintf(fo, "(Monday)</tspan>\n");
	else if(i==2) fprintf(fo, "(Tuesday)</tspan>\n");
	else if(i==3) fprintf(fo, "(Wednesday)</tspan>\n");
	else if(i==4) fprintf(fo, "(Thursday)</tspan>\n");
	else if(i==5) fprintf(fo, "(Friday)</tspan>\n");
	else if(i==6) fprintf(fo, "(Saturday)</tspan>\n");
}
char getLetter(int time) {
	if(time < 1200 && time != 0) return 'a';
	else if(time == 1200) return 'n';
	else if(time == 0 || time == 2400) return 'm';
	else return	'p';
	
}