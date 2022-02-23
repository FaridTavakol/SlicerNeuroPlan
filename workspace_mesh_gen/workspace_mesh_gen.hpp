#ifndef WORKSPACEMESHGEN_HPP_
#define WORKSPACEMESHGEN_HPP_

#include <iostream>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class WorkspaceMeshGen
{
public:
  float alpha = 10, tolerance = 0.03;
  void  GenerateMesh(std::string filename, std::string output_filename);
};

#endif