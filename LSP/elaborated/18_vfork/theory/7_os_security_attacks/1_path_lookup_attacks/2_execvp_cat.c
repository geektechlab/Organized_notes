#include <unistd.h>

int main(){

  char * args[] = {"cat",NULL};

  execvp(args[0],args);
}
