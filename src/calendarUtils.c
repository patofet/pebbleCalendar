#include <pebble.h>
#include <settings.h>

int isleap (unsigned yr) {
  return yr % 400 == 0 || (yr % 4 == 0 && yr % 100 != 0);
}

unsigned months_to_days (unsigned month) {
  return (month * 3057 - 3007) / 100;
}

void factorDate(int *month, int *year){
    
    while(*month>11){
        *month = *month - 12;
        *year = *year+1;;
    }
    while(*month<0){
        *month = *month + 12;
        *year = *year-1;;
    }
}
unsigned years_to_days (unsigned yr) {
  return yr * 365L + yr / 4 - yr / 100 + yr / 400;
}
long ymd_to_scalar (unsigned yr, unsigned mo, unsigned day) {
  long scalar;

  scalar = day + months_to_days(mo);
  if (mo > 2) /* adjust if past February */
    scalar -= isleap(yr) ? 1 : 2;
  yr--;
  scalar += years_to_days(yr);
  return scalar;
}
time_t p_mktime (struct tm *timeptr) {
  time_t tt;

  if ((timeptr->tm_year < 70) || (timeptr->tm_year > 120)) {
    tt = (time_t)-1;
  } else {
    tt = ymd_to_scalar(timeptr->tm_year + 1900,
                       timeptr->tm_mon + 1,
                       timeptr->tm_mday)
      - ymd_to_scalar(1970, 1, 1);
    tt = tt * 24 + timeptr->tm_hour;
    tt = tt * 60 + timeptr->tm_min;
    tt = tt * 60 + timeptr->tm_sec;
  }
  return tt;
}

// Calculate what day of the week it was/will be X days from the first day of the month, if mday was a wday
int wdayOfFirst(int wday,int mday){
    int a = wday - ((mday-1)%7);
    if(a<0) a += 7;
    
    return a;
}

// How many days are/were in the month
int daysInMonth(int mon, int year){
    mon++;
    
    // April, June, September and November have 30 Days
    if(mon == 4 || mon == 6 || mon == 9 || mon == 11)
        return 30;
        
    // Deal with Feburary & Leap years
    else if( mon == 2 ){
        if(year%400==0)
            return 29;
        else if(year%100==0)
            return 28;
        else if(year%4==0)
            return 29;
        else 
            return 28;
    }
    // Most months have 31 days
    else
        return 31;
}

char* traduction(char* mont){
	char* ret=months[0];
	if(strcmp(mont,"January")==0)
		ret=months[0];
	else if(strcmp(mont,"February")==0)
		ret=months[1];
	else if(strcmp(mont,"March")==0)
		ret=months[2];
	else if(strcmp(mont,"April")==0)
		ret=months[3];
	else if(strcmp(mont,"May")==0)
		ret=months[4];
	else if(strcmp(mont,"June")==0)
		ret=months[5];
	else if(strcmp(mont,"July")==0)
		ret=months[6];
	else if(strcmp(mont,"August")==0)
		ret=months[7];
	else if(strcmp(mont,"September")==0)
		ret=months[8];
	else if(strcmp(mont,"October")==0)
		ret=months[9];
	else if(strcmp(mont,"November")==0)
		ret=months[10];
	else if(strcmp(mont,"December")==0)
		ret=months[11];
	return ret;
}
