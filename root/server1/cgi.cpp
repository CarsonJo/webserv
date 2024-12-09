#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv, char **envp)
{
	printf ("<html>\n<body>\n");
  	printf ("<h2>Hello world!</h2>\n");
  	printf ("<p>Your CGI bin is set up properly</p>\n");
	 printf ("</body>\n</html>\n");
	return (0);
}
