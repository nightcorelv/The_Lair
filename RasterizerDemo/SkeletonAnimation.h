#pragma once
#include<DirectXMath.h>
#include<vector>
#include<string>
#include<debugapi.h>
#include<deque>

namespace DX = DirectX;

/// <summary>
/// Prints a string in the debug output window.
/// </summary>
/// <param name="printable"></param>
void print(std::string printable)
{
    printable += "\n";
    OutputDebugStringA(printable.c_str());
}

/// <summary>
/// Print a combination of a string and an int in the debug output window.
/// </summary>
/// <param name="printable -> needs to contain %d or %i where you want to insert the integer"></param>
/// <param name="intNum"></param>
void print(std::string printable, int intNum)
{
    printable += "\n";
    char debug[100];
    sprintf(debug, printable.c_str(), intNum);
    OutputDebugStringA(debug);
}

/// <summary>
/// Print a combination of a string and a float in the debug output window.
/// </summary>
/// <param name="printable -> needs to contain %f where you want to insert the float"></param>
/// <param name="floatNum"></param>
void print(std::string printable, float floatNum)
{
    printable += "\n";
    char debug[100];
    sprintf(debug, printable.c_str(), floatNum);
    OutputDebugStringA(debug);
}

struct Joint
{
    int parent;
    DX::XMMATRIX Bind;
    DX::XMMATRIX Model;
    DX::XMMATRIX Final;
};

/// <summary>
/// !!!Do not use as is before working out import of skeletons!!!
/// </summary>
/// <param name="joint -> should be node or something else, but will be like this for now"></param>
void process(int joint)
{
    std::vector<Joint> mySkeleton;
    std::deque<Joint> myJoints;

    //for every child, call this function again and add them to the vector/deque
    DX::XMMATRIX Bind;
    DX::XMMATRIX Model;
    DX::XMMATRIX Final;
    myJoints.push_back({ -1, Bind, Model, Final });
    myJoints.push_back({ 0, Bind, Model, Final });
    myJoints.push_back({ 1, Bind, Model, Final });
    myJoints.push_back({ 0, Bind, Model, Final });

}

//std::vector<Joint> BuildSkeleton()
//{
//    
//}

DX::XMMATRIX CalcBMITx(DX::XMMATRIX BindLocalTx, DX::XMMATRIX ParentBindModelTx)
{
    
    //Get the bind matrix for the parent (T-pose)
    //Get the bind matrix for the child (T-pose)
    //Multiply
    //Inverse
    DX::XMMATRIX BindModelTx = DX::XMMatrixMultiply(ParentBindModelTx, BindLocalTx);
    DX::XMMATRIX BindModelInverseTx = DX::XMMatrixInverse(nullptr, BindModelTx);

    return BindModelInverseTx;
}

DX::XMMATRIX CalcMTx(DX::XMMATRIX ParentModelTx, DX::XMMATRIX LocalTx)
{
    DX::XMMATRIX ModelTx = DX::XMMatrixMultiply(ParentModelTx, LocalTx);
    return ModelTx;
}

DX::XMMATRIX CalcFTx(DX::XMMATRIX ModelTx, DX::XMMATRIX BindModelInverseTx)
{
    DX::XMMATRIX FinalTx = DX::XMMatrixMultiply(ModelTx, BindModelInverseTx);
    return FinalTx;
}