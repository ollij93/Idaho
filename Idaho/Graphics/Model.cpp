// External Includes...
#include <fstream>
#include <vector>

// Includes...
#include "Model.h"
#include "Core/Assert.h"
#include "Core/Math.h"
#include "Graphics/Shader/LightShader.h"

#define MAX_MODEL_FILENAME_SIZE 256
#define MAX_MODEL_LINE_LENGTH 1024

bool
Model::Init()
{
    ASSERT(m_uModelHash != uHASH_UNSET, "Attempting to init a model before the model hash has been set.");
    if (m_uModelHash == uHASH_UNSET) { return false; }

    bool bFound = false;
    errno_t iErrno;
    WIN32_FIND_DATA xFindData;
    HANDLE hFile;
    char acFilename[MAX_MODEL_FILENAME_SIZE];

    std::ifstream xIFStream;
    char acInput[MAX_MODEL_LINE_LENGTH];

    struct FaceInfo {
        int iVertex1, iVertex2, iVertex3;
        int iTex1, iTex2, iTex3;
        int iNormal1, iNormal2, iNormal3;
    };
    std::vector<Vector3<float>> vxVertices;
    std::vector<Vector2<float>> vxTexCoords;
    std::vector<Vector3<float>> vxNormals;
    std::vector<FaceInfo> vxFaces;


    // Search the Models directory for obj files to be loaded, hash the filenames and find the one that matches
    hFile = FindFirstFile("Models/*.obj", &xFindData);
    ASSERT(hFile != INVALID_HANDLE_VALUE, "Failed to find files in the Models/ directory");
    if (hFile == INVALID_HANDLE_VALUE) { return false; }

    do {
        if (!(xFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            iErrno = strcpy_s(acFilename, MAX_MODEL_FILENAME_SIZE, "Models/");
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_MODEL_FILENAME_SIZE, xFindData.cFileName);
            }
            ASSERT(!iErrno, "Failed to copy model filename to buffer. Filename too long?");
            if (iErrno) { return false; }

            if (GetHash(acFilename) == m_uModelHash) {
                bFound = true;
            }
        }
    } while (!bFound && FindNextFile(hFile, &xFindData));
    FindClose(hFile);

    ASSERT(bFound, "Failed to find the model for the given filename");
    if (!bFound) { return false; }

    // Open the file and parse its contents
    xIFStream.open(acFilename);
    ASSERT(!xIFStream.fail(), "Failed to open the file found for the model");
    if (xIFStream.fail()) { return false; }

    xIFStream.getline(acInput, MAX_MODEL_LINE_LENGTH);
    while (!xIFStream.eof()) {
        if (strncmp(acInput, "v ", 2) == 0) {
            // The line contains information on a vertex
            Vector3<float> xVector;
            sscanf_s(acInput, "v %f %f %f", &xVector.x, &xVector.y, &xVector.z);
            vxVertices.push_back(xVector);
        } else if (strncmp(acInput, "vt", 2) == 0) {
            // The line contains information on a texture
            Vector2<float> xVector;
            sscanf_s(acInput, "vt %f %f", &xVector.x, &xVector.y);
            // DirectX uses texture coords from bottom to top not top to bottom
            xVector.y = 1.f - xVector.y;
            vxTexCoords.push_back(xVector);
        } else if (strncmp(acInput, "vn", 2) == 0) {
            // The line contains information on a normal
            Vector3<float> xVector;
            sscanf_s(acInput, "vn %f %f %f", &xVector.x, &xVector.y, &xVector.z);
            vxNormals.push_back(xVector);
        } else if (strncmp(acInput, "f ", 2) == 0) {
            // The line contains information on a face
            char acBuffer[MAX_MODEL_LINE_LENGTH];
            Vector3<int> xFaceNode;
            std::vector<Vector3<int>> vxFaceNodes;
            FaceInfo xTriFace;
            strcpy_s(acBuffer, MAX_MODEL_LINE_LENGTH, &acInput[1]);
            strcpy_s(acInput, MAX_MODEL_LINE_LENGTH, acBuffer);
            while (strlen(acBuffer) > 0) {
                for (u_int uIndex = 1; acBuffer[uIndex]; uIndex++) {
                    // Terminate the buffered string at a space
                    if (acBuffer[uIndex] == ' ') {
                        acBuffer[uIndex] = 0;
                    }
                }
                // acBuffer now contains just " %d/%d/%d"
                sscanf_s(acBuffer, " %d/%d/%d", &xFaceNode.x, &xFaceNode.y, &xFaceNode.z);
                vxFaceNodes.push_back(xFaceNode);

                // Copy the rest of the line into the buffer and input and continue
                strcpy_s(acBuffer, MAX_MODEL_LINE_LENGTH, &acInput[strlen(acBuffer)]);
                strcpy_s(acInput, MAX_MODEL_LINE_LENGTH, acBuffer);
            }
            // Construct FaceInfo objects for each triangular face in the polygon face
            for (u_int uIndex = 0; uIndex < vxFaceNodes.size() - 2; uIndex++) {
                xTriFace.iVertex1 = vxFaceNodes[0].x;
                xTriFace.iTex1 = vxFaceNodes[0].y;
                xTriFace.iNormal1 = vxFaceNodes[0].z;

                xTriFace.iVertex2 = vxFaceNodes[2 + uIndex].x;
                xTriFace.iTex2 = vxFaceNodes[2 + uIndex].y;
                xTriFace.iNormal2 = vxFaceNodes[2 + uIndex].z;

                xTriFace.iVertex3 = vxFaceNodes[1 + uIndex].x;
                xTriFace.iTex3 = vxFaceNodes[1 + uIndex].y;
                xTriFace.iNormal3 = vxFaceNodes[1 + uIndex].z;

                vxFaces.push_back(xTriFace);
            }
        }
        xIFStream.getline(acInput, MAX_MODEL_LINE_LENGTH);
    }

    // Loop through the faces and use this to fill in the vertex info
    m_uVertexCount = 3 * vxFaces.size();
    m_pxVertices = new Vertex[m_uVertexCount];
    for (u_int u = 0; u < vxFaces.size(); u++) {
        m_pxVertices[3 * u].m_xPosition = vxVertices[vxFaces[u].iVertex1 - 1];
        m_pxVertices[3 * u].m_xNormal = vxNormals[vxFaces[u].iNormal1 - 1];
        m_pxVertices[3 * u].m_xTexture = vxTexCoords[vxFaces[u].iTex1 - 1];

        m_pxVertices[3 * u + 1].m_xPosition = vxVertices[vxFaces[u].iVertex2 - 1];
        m_pxVertices[3 * u + 1].m_xNormal = vxNormals[vxFaces[u].iNormal2 - 1];
        m_pxVertices[3 * u + 1].m_xTexture = vxTexCoords[vxFaces[u].iTex2 - 1];

        m_pxVertices[3 * u + 2].m_xPosition = vxVertices[vxFaces[u].iVertex3 - 1];
        m_pxVertices[3 * u + 2].m_xNormal = vxNormals[vxFaces[u].iNormal3 - 1];
        m_pxVertices[3 * u + 2].m_xTexture = vxTexCoords[vxFaces[u].iTex3 - 1];
    }

    return PARENT::Init();
}

void
Model::Shutdown()
{
    if (m_pxVertices) {
        delete[] m_pxVertices;
        m_pxVertices = nullptr;
    }
}
