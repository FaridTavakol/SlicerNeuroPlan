#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCellIterator.h>
#include <vtkCleanPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkDelaunay3D.h>
#include <vtkMath.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPLYWriter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSimplePointsReader.h>
#include <vtkSimplePointsWriter.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnstructuredGrid.h>

#include "workspace_mesh_gen.hpp"

using namespace std;
void WorkspaceMeshGen::GenerateMesh(string filename, string output_filename)
{
  vtkNew< vtkNamedColors >   colors;
  vtkNew< vtkCleanPolyData > originalPts;

  std::cout << "Reading file." << std::endl;
  // Read the file
  vtkNew< vtkSimplePointsReader > reader;
  reader->SetFileName(filename.c_str());
  reader->Update();
  std::cout << "Read." << std::endl;
  originalPts->SetInputConnection(reader->GetOutputPort());

  originalPts->Update();
  auto pts = originalPts->GetOutput()->GetNumberOfPoints();
  std::cout << "Read." << std::endl;
  originalPts->SetTolerance(tolerance);
  originalPts->Update();
  auto cleanPts = originalPts->GetOutput()->GetNumberOfPoints();

  std::cout << "Original points " << pts << std::endl;
  std::cout << "Downsampled points " << cleanPts << std::endl;

  vtkNew< vtkDelaunay3D > delaunay3D;
  delaunay3D->SetInputConnection(originalPts->GetOutputPort());
  // delaunay3D->SetInputData(originalPts);

  // Generate a mesh from the input points. If Alpha is non-zero, then
  // tetrahedra, triangles, edges and vertices that lie within the
  // alpha radius are output.
  vtkNew< vtkDelaunay3D > delaunay3DAlpha;
  delaunay3DAlpha->SetInputConnection(originalPts->GetOutputPort());
  // delaunay3DAlpha->SetInputData(originalPts);
  delaunay3DAlpha->SetAlpha(alpha);
  delaunay3DAlpha->Update();

  auto surfaceFilter = vtkSmartPointer< vtkDataSetSurfaceFilter >::New();
  surfaceFilter->SetInputConnection(delaunay3DAlpha->GetOutputPort());
  surfaceFilter->Update();

  vtkPolyData* polydata = surfaceFilter->GetOutput();

  std::string            output_fname = output_filename;
  vtkNew< vtkPLYWriter > plyWriter;
  plyWriter->SetFileName(output_fname.c_str());
  plyWriter->SetInputConnection(delaunay3DAlpha->GetOutputPort());
  plyWriter->SetInputData(polydata);
  plyWriter->Write();
}
