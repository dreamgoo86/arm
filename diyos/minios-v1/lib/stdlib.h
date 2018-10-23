/*
 * File:		stdlib.h
 * Purpose:		Function prototypes for standard library functions
 *
 * Notes:
 */

#ifndef _STDLIB_H
#define _STDLIB_H

/********************************************************************
 * Standard library functions
 ********************************************************************/

int
isspace (int);

int
isalnum (int);

int
isdigit (int);

int
isupper (int);

int
strcasecmp (const char *, const char *);

int
strncasecmp (const char *, const char *, int);

int
strnicmp (const char *s1, const char *s2, int n);

unsigned long
strtoul (char *, char **, int);

int
atoi (const char *str);

int
strlen (const char *);

char *
strcat (char *, const char *);

char *
strncat (char *, const char *, int);

char *
strcpy (char *, const char *);

char *
strncpy (char *, const char *, int);

int
strcmp (const char *, const char *);

int
strncmp (const char *, const char *, int);

char *
strstr (const char *, const char *);

int
memcmp (const void *, const void *, unsigned);

void *
memcpy (void *, const void *, unsigned);

void *
memset (void *, int, unsigned);

void
free (void *);

void *
malloc (unsigned);

#define RAND_MAX 32767

int
rand (void);

void
srand (int);

int	printf(const char *fmt, ...);
int	nprintf(char *str, int siz, const char *fmt, ...);
int	snprintf(char *str, int siz, const char *fmt, ...);

/********************************************************************/

#endif
