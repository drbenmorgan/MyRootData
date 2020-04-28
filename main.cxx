#include "MyData.h"

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include <iostream>

// Consts for Tree/Branch name/desc
const char kTreeName[] = "MyDataTree";
const char kTreeDesc[] = "Tree of waveform data";
const char kBranchName[] = "Waveforms";

// Implementation decls
int read(const char* filename);
int write(const char* filename);

// Core main
int main(int argc, char* argv[]) {
  // Noddy CLI
  if (argc != 3) {
    std::cerr << "usage: MyDataRW <action> <file>\n";
    return 1;
  }

  std::string action = argv[1];
  if (action == "read") {
    return read(argv[2]);
  }
  else if (action == "write") {
    return write(argv[2]);
  }
  else {
    std::cerr << "invalid action '" << action << "'\n";
    return 1;
  }
}

// Implementation defs
int read(const char* filename) {
  TFile f{filename, "OPEN"};
  if (!f.IsOpen()) {
    std::cerr << "Error: could not open requested file '"
              << filename << "'" << std::endl;
    return 1;
  }

  // These are the simplest way to read data!
  // https://root.cern/doc/v618/classTTreeReader.html
  // Even better are dataframes:
  // https://root.cern/doc/v618/classROOT_1_1RDataFrame.html
  TTreeReader reader(kTreeName, &f);
  TTreeReaderValue<MyData> d(reader, kBranchName);

  while (reader.Next()) {
    std::cout << "entry: ("
              << (*d).a.size()
              << ", "
              << (*d).b.size()
              << ")\n";
  }
  return 0;
}

int write(const char* filename) {
  // Recreate file
  TFile f{filename, "RECREATE"};

  // Make Tree
  TTree t{kTreeName, kTreeDesc};

  // Object and branch
  //auto d = new MyData;
  MyData d;
  t.Branch(kBranchName, "MyData", &d);

  for (size_t i=0; i < 10; ++i) {
    // Should add data here, or reset/create object as needed
    d.a.assign(i+1, i);
    d.b.assign(i+1, i*2);

    // Fill...
    t.Fill();
  }

  // save the Tree header; the file will be automatically closed
  // when going out of the function scope
  t.Write();
  return 0;
}
