#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {LOOKING, WRITING} action;
typedef enum {TRKPT, LAT, LON, ELE_START, ELE_END, TIME_START, TIME_END} tag;

/*
Set of action states:
LOOKING: Actively looking for (i.e. not writing) certain character (specified by "tag" states)
WRITING: Actively writing chars from input stream to output stream

Set of tag states:
TRKPT: <trkpt> (not case senstitive)
LAT: lat=("/') (case sensitive)
LON: lon=("/') (case sensitive)
ELE_START: <ele (not case sensitive)
ELE_END: >
TIME_START: <time (not case sensitive)
TIME_END: >

General Rules:
- When an action state is LOOKING: it is currently looking for whatever tag state it is in
- When an action state is WRITING: it is currently writing, and the tag state it is in designates 
the next tag it will look for when it returns to the LOOKING STATE
	- That means that the (WRITING,LON) state does NOT mean that the program is writing the longitude, 
	it means that it is writing whatever property was there before (in this case, LAT), and it PREPARED 
	to LOOK for the lon attribute once it goes back to the LOOKING state.
*/

int main(int argc, char **argv)
{
    //current "co-states" of FSM
	action curr_action = LOOKING;
	tag curr_tag = TRKPT;

	//strings to store lower- and uppercase versions of the text that signals checkpoints in the .gpx file where states change
	//if the checkpoint string is case sensitive, then both wanted_string_lower and wanted_string_upper will be identical
	//the strings length is also a variable so that the program can easily compare all of the checkpoint string's characters have
	//been consecutively written
	char *wanted_string_lower = "<trkpt";
	char *wanted_string_upper = "<TRKPT";
	int wanted_string_length = 6;

	//variables to check whether either the lat or lon attributes are actually nested in another attribute's value
	int in_nested_quotes = 0;
	int diff_att = 0;
	char in_nested_quotes_type = '\'';

	//index of checkpoint string to be checked
	int index = 0;

	//char that signals WRITING state to stop writing and switch to reading
	char exit_char = '\"';

	//holds the character that is currently being read by scanf
	char buffer;

	while (scanf("%c", &buffer) != EOF)	
	{
		// isspace takes care of the whitespace edge cases
		if (isspace(buffer) == 0) 
		{
			switch (curr_action)
			{
				case LOOKING:
					if ((buffer == wanted_string_lower[index]) || (buffer == wanted_string_upper[index]))
					{	
						if (index + 1 == wanted_string_length)
						{
							//go on to LOOKING for another tag or WRITING the buffer to stdout
							//change wanted_string_lower, wanted_string_upper, wanted_string_length, exit_char (if necessary)
							switch (curr_tag)
							{
								case TRKPT:
									curr_tag = LAT;
									wanted_string_lower = "lat=\'";
									wanted_string_upper = "lat=\"";
									wanted_string_length = 5;
									//starts LOOKING for LAT
								break;

								case LAT:
									if (diff_att == 0)
									{
										curr_action = WRITING;
										curr_tag = LON;
										wanted_string_lower = "lon=\'";
										wanted_string_upper = "lon=\"";
										wanted_string_length = 5;
										if (buffer=='\'')
										{
											exit_char = '\'';
										}
										else
										{
											exit_char = '\"';
										}	
										//starts WRITING from LAT
									}
								break;

								case LON:
									if (diff_att == 0)
									{
										curr_action = WRITING;
										curr_tag = ELE_START;
										wanted_string_lower = "<ele";
										wanted_string_upper = "<ELE";
										wanted_string_length = 4;
										if (buffer=='\'')
										{
											exit_char = '\'';
										}
										else
										{
											exit_char = '\"';
										}
										//starts WRITING from LON
									}
								break;

								case ELE_START:
									curr_tag = ELE_END;
									wanted_string_lower = ">";
									wanted_string_upper = ">";
									wanted_string_length = 1;
									exit_char = '<';
									//starts LOOKING for ELE_END (">")
								break;

								case ELE_END:
									curr_action = WRITING;
									curr_tag = TIME_START;
									wanted_string_lower = "<time";
									wanted_string_upper = "<TIME";
									wanted_string_length = 5;
									//starts WRITING from ELE_END
								break;

								case TIME_START:
									curr_tag = TIME_END;
									wanted_string_lower = ">";
									wanted_string_upper = ">";
									wanted_string_length = 1;
									//starts LOOKING for TIME_END (">")
								break;

								case TIME_END:
									curr_action = WRITING;
									curr_tag = TRKPT;
									wanted_string_lower = "<trkpt";
									wanted_string_upper = "<TRKPT";
									wanted_string_length = 6;
									//starts WRITING from TIME_END
							}
							index = 0; //resets index so it knows to look for the first character of the checkpoint string
						}
						else
						{
							index++; //moves on to the next character if both characters match but hasn't reached end yet
						}
					}
					else //buffer doesn't match up with char
					{
						if ((curr_tag == LAT) || (curr_tag == LON))
						{
							if (diff_att == 0)
							{
								diff_att = 1;
							}
						}
						index = 0; //program has to start matching first character of checkpoint string again
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
			if (((curr_tag == LAT) || (curr_tag == LON)) && diff_att == 1)
			{
				if ((buffer == '\"' || buffer == '\'') && in_nested_quotes == 0)
				{
					in_nested_quotes = 1;
					in_nested_quotes_type = buffer;
				}
				else if (buffer == in_nested_quotes_type && in_nested_quotes == 1)
				{
					in_nested_quotes = 0;
					diff_att = 0;
				}
			}
		}
	}
	return 0;
}
