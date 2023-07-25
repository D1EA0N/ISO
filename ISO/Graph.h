#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;

namespace Graphs {

    public ref class Graph
    {
    private:
        int numVertices;
        array<array<int>^>^ adjacencyMatrix;

        ref class Edge
        {
        public:
            int start;
            int end;

            Edge(int start, int end)
            {
                this->start = start;
                this->end = end;
            }
        };
        Stack<Edge^>^ operationHistory;
    public:

        Graph(int vertices)
        {
            numVertices = vertices;
            InitializeAdjacencyMatrix();
            operationHistory = gcnew Stack<Edge^>();
        }
        void InitializeAdjacencyMatrix()
        {
            adjacencyMatrix = gcnew array<array<int>^>(numVertices);
            for (int i = 0; i < numVertices; i++)
            {
                adjacencyMatrix[i] = gcnew array<int>(numVertices);
                for (int j = 0; j < numVertices; j++)
                {
                    adjacencyMatrix[i][j] = 0; // Initialize all elements to 0
                }
            }
        }

        //Retaining the data
        void ResizeAdjacencyMatrix(int newVertices)
        {
            // Create a temporary matrix to hold the existing data
            array<array<int>^>^ tempMatrix = gcnew array<array<int>^>(newVertices);
            for (int i = 0; i < newVertices; i++)
            {
                tempMatrix[i] = gcnew array<int>(newVertices);
                for (int j = 0; j < newVertices; j++)
                {
                    if (i < numVertices && j < numVertices)
                    {
                        // Copy existing data to the temporary matrix
                        tempMatrix[i][j] = adjacencyMatrix[i][j];
                    }
                    else
                    {
                        // Fill new rows and columns with zeros
                        tempMatrix[i][j] = 0;
                    }
                }
            }

            // Replace the adjacency matrix with the resized matrix
            adjacencyMatrix = tempMatrix;
            numVertices = newVertices;
        }

        void AddEdge(int start, int end)
        {
            adjacencyMatrix[start][end] += 1;
            adjacencyMatrix[end][start] += 1;

            operationHistory->Push(gcnew Edge(start, end));
        }
        void Undo()
        {
            if (operationHistory->Count > 0)
            {
                Edge^ lastOperation = operationHistory->Pop();
                int start = lastOperation->start;
                int end = lastOperation->end;
                // Remove the edge from the adjacency matrix
                adjacencyMatrix[start][end] = 0;
                adjacencyMatrix[end][start] = 0;

            }
        }
        void RemoveDisconnectedVertices()
        {
            List<int>^ verticesToRemove = gcnew List<int>();

            for (int i = 0; i < numVertices; i++)
            {
                bool hasEdges = false;
                for (int j = 0; j < numVertices; j++)
                {
                    if (adjacencyMatrix[i][j] != 0)
                    {
                        hasEdges = true;
                        break;
                    }
                }
                if (hasEdges == false)
                {
                    verticesToRemove->Add(i);
                    break;
                }
            }

            if (verticesToRemove->Count > 0)
            {
                // Remove the rows and columns of disconnected vertices
                for (int i = 0; i < verticesToRemove->Count; i++)
                {
                    int vertexToRemove = verticesToRemove[i];
                    for (int j = 0; j < numVertices; j++)
                    {
                        adjacencyMatrix[vertexToRemove][j] = 0;
                        adjacencyMatrix[j][vertexToRemove] = 0;
                    }
                }
                // Resize the adjacency matrix after removing the disconnected vertices
                ResizeAdjacencyMatrix(numVertices - verticesToRemove->Count);
            }
        }
        array<array<int>^>^ GetAdjacencyMatrix()
        {
            return adjacencyMatrix;
        }
    };
}