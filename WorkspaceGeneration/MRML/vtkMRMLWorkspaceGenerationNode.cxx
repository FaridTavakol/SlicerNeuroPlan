// WorkspaceGeneration includes
#include "vtkMRMLWorkspaceGenerationNode.h"

// slicer includes
#include "vtkMRMLDisplayNode.h"
#include "vtkMRMLMarkupsDisplayNode.h"
#include "vtkMRMLModelDisplayNode.h"
#include "vtkMRMLModelNode.h"

// Other MRML includes
#include "vtkMRMLNode.h"

// VTK includes
#include <vtkNew.h>

// Other includes
#include <iostream>
#include <sstream>
#include <string.h>

static const char* INPUT_ROLE = "InputNode";
static const char* OUTPUT_MODEL_ROLE = "OutputModel";

vtkMRMLNodeNewMacro(vtkMRMLWorkspaceGenerationNode);

//-----------------------------------------------------------------
vtkMRMLWorkspaceGenerationNode::vtkMRMLWorkspaceGenerationNode()
{
  this->HideFromEditorsOff();
  this->SetSaveWithScene(true);

  vtkNew< vtkIntArray > events;
  events->InsertNextValue(vtkCommand::ModifiedEvent);
  events->InsertNextValue(vtkMRMLModelNode::MeshModifiedEvent);

  this->AddNodeReferenceRole(INPUT_ROLE, NULL, events.GetPointer());
  this->AddNodeReferenceRole(OUTPUT_MODEL_ROLE);

  this->AutoUpdateOutput = true;
  this->InputNodeType = NONE;
}

//-----------------------------------------------------------------
vtkMRMLWorkspaceGenerationNode::~vtkMRMLWorkspaceGenerationNode()
{
}

//-----------------------------------------------------------------
void vtkMRMLWorkspaceGenerationNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);  // This will take care of referenced nodes
  vtkMRMLWriteXMLBeginMacro(of);
  vtkMRMLWriteXMLBooleanMacro(AutoUpdateOutput, AutoUpdateOutput);
  vtkMRMLWriteXMLIntMacro(InputNodeType, InputNodeType);
  vtkMRMLWriteXMLEndMacro();
}

//-----------------------------------------------------------------
void vtkMRMLWorkspaceGenerationNode::ReadXMLAttributes(const char** atts)
{
  int disabledModify = this->StartModify();
  Superclass::ReadXMLAttributes(atts);  // This will take care of referenced
                                        // nodes
  vtkMRMLReadXMLBeginMacro(atts);
  vtkMRMLReadXMLBooleanMacro(AutoUpdateOutput, AutoUpdateOutput);
  vtkMRMLReadXMLBooleanMacro(InputNodeType, InputNodeType);
  vtkMRMLReadXMLEndMacro();
  this->EndModify(disabledModify);
}

//-----------------------------------------------------------------
void vtkMRMLWorkspaceGenerationNode::Copy(vtkMRMLNode* anode)
{
  int disabledModify = this->StartModify();
  Superclass::Copy(anode);  // This will take care of referenced nodes
  vtkMRMLCopyBeginMacro(anode);
  vtkMRMLCopyBooleanMacro(AutoUpdateOutput);
  vtkMRMLCopyBooleanMacro(InputNodeType);
  vtkMRMLCopyEndMacro();
  this->EndModify(disabledModify);
}

//-----------------------------------------------------------------
void vtkMRMLWorkspaceGenerationNode::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);  // This will take care of referenced nodes
  vtkMRMLPrintBeginMacro(os, indent);
  vtkMRMLPrintBooleanMacro(AutoUpdateOutput);
  vtkMRMLPrintBooleanMacro(InputNodeType);
  vtkMRMLPrintEndMacro();
}

//-----------------------------------------------------------------
vtkMRMLNode* vtkMRMLWorkspaceGenerationNode::GetInputNode()
{
  vtkMRMLNode* inputNode = this->GetNodeReference(INPUT_ROLE);
  return inputNode;
}

//-----------------------------------------------------------------
vtkMRMLModelNode* vtkMRMLWorkspaceGenerationNode::GetOutputModelNode()
{
  vtkMRMLModelNode* modelNode =
    vtkMRMLModelNode::SafeDownCast(this->GetNodeReference(OUTPUT_MODEL_ROLE));
  return modelNode;
}

//-----------------------------------------------------------------
void vtkMRMLWorkspaceGenerationNode::SetAndObserveInputNodeID(
  const char* inputId, int input_node_type)
{
  // error check
  const char* outputId = this->GetNodeReferenceID(OUTPUT_MODEL_ROLE);
  if (inputId != NULL && outputId != NULL && strcmp(inputId, outputId) == 0)
  {
    vtkErrorMacro("Input node and output node cannot be the same.");
    return;
  }

  this->InputNodeType = input_node_type;

  this->SetAndObserveNodeReferenceID(INPUT_ROLE, inputId);
}

//-----------------------------------------------------------------
void vtkMRMLWorkspaceGenerationNode::SetAndObserveOutputModelNodeID(
  const char* outputId)
{
  // error check
  const char* inputId = this->GetNodeReferenceID(INPUT_ROLE);
  if (inputId != NULL && outputId != NULL && strcmp(inputId, outputId) == 0)
  {
    vtkErrorMacro("Input node and output node cannot be the same.");
    return;
  }

  this->SetAndObserveNodeReferenceID(OUTPUT_MODEL_ROLE, outputId);
}

//-----------------------------------------------------------------
// void vtkMRMLWorkspaceGenerationNode::ProcessMRMLEvents(vtkObject* caller,
//                                                        unsigned long
//                                                        /*event*/, void*
//                                                        /*callData*/)
// {
//   vtkInfoMacro("Entered ProcessMRML Events Callback");
//   vtkMRMLNode* callerNode = vtkMRMLNode::SafeDownCast(caller);
//   if (callerNode == NULL)
//     return;

//   if (this->GetInputNode() && this->GetInputNode() == caller)
//   {
//     // this->InvokeCustomModifiedEvent(MarkupsPositionModifiedEvent);
//   }
// }