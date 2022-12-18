#include "interface.h"
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {

  if (argc > 2) {
    vector<string> filenames;
    for (int i = 1; i < argc; ++i) {
      filenames.emplace_back(argv[i]);
    }
    Interface gui(filenames);
    gui.Run();
  }
  return 0;
}
