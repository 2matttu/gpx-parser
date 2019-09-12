#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {LOOKING, WRITING} action;
typedef enum {TRKPT, LAT, LON, ELE_START, TIME_START} tag;

int main(int argc, char **argv)
{
    //current state of FSM
	action curr_action = LOOKING;
	tag curr_tag = TRKPT;

	char *wanted_string_lower = "<trkpt";
	char *wanted_string_upper = "<TRKPT";
	int wanted_string_length = 6;

	int index = 0;

	char exit_char = '\"';

	char buffer;

	while (scanf("%c", &buffer) != EOF)	
	{
		// printf("%c", buffer);
		
		if (isspace(buffer) == 0) {
		// isprint takes care of the whitespace edge cases

			switch (curr_action)
			{
				case LOOKING:
					if ((buffer == wanted_string_lower[index]) || (buffer == wanted_string_upper[index]))
					{
						if (index + 1 == wanted_string_length)
						{
							//i.e. go on to LOOKING for another tag or WRITING the buffer to stdout
							//change wanted_string_lower, wanted_string_upper, wanted_string_length, exit_char
							switch (curr_tag)
							{
								case TRKPT:
									curr_tag = LAT;
									wanted_string_lower = "lat=\"";
									wanted_string_upper = "LAT=\"";
									wanted_string_length = 5;
									exit_char = '\"';
									//starts LOOKING for LAT
								break;

								case LAT:
									curr_action = WRITING;
									curr_tag = LON;
									wanted_string_lower = "lon=\"";
									wanted_string_upper = "LON=\"";
									wanted_string_length = 5;
									//starts WRITING from LAT
								break;

								case LON:
									curr_action = WRITING;
									curr_tag = ELE_START;
									wanted_string_lower = "<ele>";
									wanted_string_upper = "<ELE>";
									wanted_string_length = 5;
									//starts WRITING from LON
								break;

								case ELE_START:
									curr_action = WRITING;
									curr_tag = TIME_START;
									wanted_string_lower = "<time>";
									wanted_string_upper = "<TIME>";
									wanted_string_length = 6;
									exit_char = '<';
									//starts WRITING from ELE_START
								break;

								case TIME_START:
									curr_action = WRITING;
									curr_tag = TRKPT;
									wanted_string_lower = "<trkpt";
									wanted_string_upper = "<TRKPT";
									wanted_string_length = 6;
									//starts WRITING from TIME_START
								break;
							}
							index = 0;
						}
						else
						{
							index++;
						}
					}
					else
					{
						index = 0;
					}
				break;

				case WRITING:
					if (buffer == exit_char)
					{
						curr_action = LOOKING;
						if (curr_tag == TRKPT)
						{
							printf("\n"); //prints newline before each latitude readout (i.e. when curr_tag equals TRKPT)
						}
						else
						{
							printf(",");
						}
						//LOOKING cases take care of changing curr_tag, wanted_string and so on
					}
					else
					{
						if ((curr_tag == TRKPT) && (buffer == ','))
						{
							printf("&comma;");
						}
						else
						{
							printf("%c", buffer);
						}
					}
				break;	
			}
		}
	}
	return 0;
}
