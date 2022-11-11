#include "mesh.h"
#include "cylinder.h"
#include "matrix.h"

/*!
\class Mesh mesh.h

\brief Core triangle mesh class.
*/



/*!
\brief Initialize the mesh to empty.
*/
Mesh::Mesh()
{
}

/*!
\brief Initialize the mesh from a list of vertices and a list of triangles.

Indices must have a size multiple of three (three for triangle vertices and three for triangle normals).

\param vertices List of geometry vertices.
\param indices List of indices wich represent the geometry triangles.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<int>& indices) :vertices(vertices), varray(indices)
{
  normals.resize(vertices.size(), Vector::Z);
}

/*!
\brief Create the mesh.

\param vertices Array of vertices.
\param normals Array of normals.
\param va, na Array of vertex and normal indexes.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<Vector>& normals, const std::vector<int>& va, const std::vector<int>& na) :vertices(vertices), normals(normals), varray(va), narray(na)
{
}

/*!
\brief Reserve memory for arrays.
\param nv,nn,nvi,nvn Number of vertices, normals, vertex indexes and vertex normals.
*/
void Mesh::Reserve(int nv, int nn, int nvi, int nvn)
{
  vertices.reserve(nv);
  normals.reserve(nn);
  varray.reserve(nvi);
  narray.reserve(nvn);
}

/*!
\brief Empty
*/
Mesh::~Mesh()
{
}

/*!
\brief Smooth the normals of the mesh.

This function weights the normals of the faces by their corresponding area.
\sa Triangle::AreaNormal()
*/
void Mesh::SmoothNormals()
{
  // Initialize 
  normals.resize(vertices.size(), Vector::Null);

  narray = varray;

  // Accumulate normals
  for (size_t i = 0; i < varray.size(); i += 3)
  {
    Vector tn = Triangle(vertices[varray.at(i)], vertices[varray.at(i + 1)], vertices[varray.at(i + 2)]).AreaNormal();
    normals[narray[i + 0]] += tn;
    normals[narray[i + 1]] += tn;
    normals[narray[i + 2]] += tn;
  }

  // Normalize 
  for (size_t i = 0; i < normals.size(); i++)
  {
    Normalize(normals[i]);
  }
}

/*!
\brief Add a smooth triangle to the geometry.
\param a, b, c Index of the vertices.
\param na, nb, nc Index of the normals.
*/
void Mesh::AddSmoothTriangle(int a, int na, int b, int nb, int c, int nc)
{
  varray.push_back(a);
  narray.push_back(na);
  varray.push_back(b);
  narray.push_back(nb);
  varray.push_back(c);
  narray.push_back(nc);
}

/*!
\brief Add a triangle to the geometry.
\param a, b, c Index of the vertices.
\param n Index of the normal.
*/
void Mesh::AddTriangle(int a, int b, int c, int n)
{
  varray.push_back(a);
  narray.push_back(n);
  varray.push_back(b);
  narray.push_back(n);
  varray.push_back(c);
  narray.push_back(n);
}

/*!
\brief Add a smmoth quadrangle to the geometry.

Creates two smooth triangles abc and acd.

\param a, b, c, d  Index of the vertices.
\param na, nb, nc, nd Index of the normal for all vertices.
*/
void Mesh::AddSmoothQuadrangle(int a, int na, int b, int nb, int c, int nc, int d, int nd)
{
  // First triangle
  AddSmoothTriangle(a, na, b, nb, c, nc);

  // Second triangle
  AddSmoothTriangle(a, na, c, nc, d, nd);
}

/*!
\brief Add a quadrangle to the geometry.

\param a, b, c, d  Index of the vertices and normals.
*/
void Mesh::AddQuadrangle(int a, int b, int c, int d)
{
  AddSmoothQuadrangle(a, a, b, b, c, c, d, d);
}

/*!
\brief Compute the bounding box of the object.
*/
Box Mesh::GetBox() const
{
  if (vertices.size() == 0)
  {
    return Box::Null;
  }
  return Box(vertices);
}

/*!
\brief Creates an axis aligned box.

The object has 8 vertices, 6 normals and 12 triangles.
\param box The box.
*/
Mesh::Mesh(const Box& box)
{
  // Vertices
  vertices.resize(8);

  for (int i = 0; i < 8; i++)
  {
    vertices[i] = box.Vertex(i);
  }

  // Normals
  normals.push_back(Vector(-1, 0, 0));
  normals.push_back(Vector(1, 0, 0));
  normals.push_back(Vector(0, -1, 0));
  normals.push_back(Vector(0, 1, 0));
  normals.push_back(Vector(0, 0, -1));
  normals.push_back(Vector(0, 0, 1));

  // Reserve space for the triangle array
  varray.reserve(12 * 3);
  narray.reserve(12 * 3);

  AddTriangle(0, 2, 1, 4);
  AddTriangle(1, 2, 3, 4);

  AddTriangle(4, 5, 6, 5);
  AddTriangle(5, 7, 6, 5);

  AddTriangle(0, 4, 2, 0);
  AddTriangle(4, 6, 2, 0);

  AddTriangle(1, 3, 5, 1);
  AddTriangle(3, 7, 5, 1);

  AddTriangle(0, 1, 5, 2);
  AddTriangle(0, 5, 4, 2);

  AddTriangle(3, 2, 7, 3);
  AddTriangle(6, 7, 2, 3);
}

Mesh::Mesh(const Disc& disc, int n)
{
    // Vertices
    double angle = 2 * M_PI / n;
    vertices.push_back(disc.Center());
    for (int i = 0; i < n; ++i)
    {
        double a = angle * i;
        vertices.push_back(Vector(cos(a), sin(a), 0));
    }

    // Normals
    normals.push_back(disc.Normal());

    // Topology
    for (int i = 1; i < n; i++)
    {
        AddTriangle(i, i+1, 0, 0);
    }
    AddTriangle(0, 1, n, 0);
}

Mesh::Mesh(const Cone& cone, int n)
{
    // Vertices
    double angle = 2 * M_PI / n;
    vertices.push_back(cone.Center());
    vertices.push_back(cone.Center() + cone.Normal() * cone.Height());
    for (int i = 0; i < n; ++i)
    {
        double a = angle * i;
        vertices.push_back(Vector(cos(a), sin(a), 0));
    }

    // TODO: Normals
    normals.push_back(cone.Normal());

    // Topology
    for (int i = 2; i < n+1; ++i)
    {
        AddTriangle(0, i, i+1, 0);
    }
    AddTriangle(0, 2, n+1, 0);
    for (int i = 2; i < n+1; ++i)
    {
        AddTriangle(1, i, i+1, 0);
    }
    AddTriangle(1, 2, n+1, 0);
}

Mesh::Mesh(const Cylinder& cylinder, int n)
{
    // Vertices
    double angle = 2 * M_PI / n;
    vertices.push_back(cylinder.Center()); // Index 0
    for (int i = 0; i < n; ++i)
    {
        double a = angle * i;
        vertices.push_back(Vector(cos(a), sin(a), 0));
    }
    vertices.push_back(cylinder.Center() + cylinder.Normal() * cylinder.Height()); // Index n+1
    for (int i = 0; i < n; ++i)
    {
        double a = angle * i;
        vertices.push_back(Vector(cos(a), sin(a), cylinder.Height()));
    }

    // TODO: Normals
    normals.push_back(cylinder.Normal());

    // Topology
    for (int i = 1; i < n; ++i)
    {
        AddTriangle(0, i, i+1, 0);
    }
    AddTriangle(0, 1, n, 0);
    for (int i = n+2; i < 2*n+1; ++i)
    {
        AddTriangle(n+1, i, i+1, 0);
    }
    AddTriangle(n+1, n+2, 2*n+1, 0);
    for (int i = 1; i < n; ++i)
    {
        AddTriangle(i, i+1, n+i+1, 0);
    }
    AddTriangle(n, 1, 2*n+1, 0);
    for (int i = 1; i < n; ++i)
    {
        AddTriangle(i+n+1, i+n+2, i+1, 0);
    }
    AddTriangle(2*n+1, n+2, 1, 0);
}

Mesh::Mesh(const Sphere& sphere,  int n)
{
    // Vertices
    double angle = 2 * M_PI / n;
    vertices.push_back(sphere.Center() - Vector(0, 0, 1) * sphere.Radius()); // Index 0
    vertices.push_back(sphere.Center() + Vector(0, 0, 1) * sphere.Radius()); // Index 1
    for (int i = 1; i < n-1; ++i)
    {
        double ai = (M_PI /n) * i;
        for (int j = 0; j < n; ++j)
        {
            double aj = angle * j;
            vertices.push_back(Vector(cos(aj)*sin(ai), sin(aj)*sin(ai), i*2/n));
        }
    }

    // Normals
    normals.push_back(Vector(0.0, 0.0, 1.0));

    // Topology
    for (int j = 2; j < n+1; ++j)
    {
        AddTriangle(0, j, j+1, 0);
    }
    AddTriangle(0, n+1, 2, 0);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 2; j < n+1; ++j)
        {
            AddTriangle(i*n+j, (i+1)*n+j, (i+1)*n+j+1, 0);
            AddTriangle(i*n+j, i*n+j+1, (i+1)*n+j+1, 0);
        }
    }

    for (int j = 2; j < n+1; ++j)
    {
        AddTriangle(1, (n+1)*n+j, (n+1)*n+j+1, 0);
    }
    AddTriangle(1, (n+1)*n+n+1, (n+1)*n+2, 0);
}

Mesh::Mesh(const Tore& tore, int n)
{
    // TODO:
}

Mesh::Mesh(const Capsule& capsule, int n)
{
    // TODO:
}

/*!
\brief Scale the mesh.
\param s Scaling factor.
*/
void Mesh::Scale(double s)
{
    // Vertexes
    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i] *= s;
    }

    if (s < 0.0)
    {
        // Normals
        for (size_t i = 0; i < normals.size(); i++)
        {
            normals[i] = -normals[i];
        }
    }
}

void Mesh::Translate(Vector& t)
{
    for(size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] += t;
    }
}

void Mesh::RotateX(double a)
{
    Matrix m = Matrix::getRotationX(a);

    // Vertices
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        std::cout << vertices[i] << " to " << m * vertices[i] << std::endl;
        vertices[i] = m * vertices[i];
    }

    // Normals
    for (size_t i = 0; i < normals.size(); ++i)
    {
        normals[i] = m * vertices[i];
    }
}

void Mesh::RotateY(double a)
{
    Matrix m = Matrix::getRotationY(a);

    // Vertices
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = m * vertices[i];
    }

    // Normals
    for (size_t i = 0; i < normals.size(); ++i)
    {
        normals[i] = m * normals[i];
    }
}

void Mesh::RotateZ(double a)
{
    Matrix m = Matrix::getRotationZ(a);

    // Vertices
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = m * vertices[i];
    }

    // Normals
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        normals[i] = m * normals[i];
    }
}

// TODO: Fix merge (not working)
void Mesh::Merge(Mesh& m)
{
    int initial_size = 0;
    // Vertices
    initial_size = vertices.size();
    vertices.resize(vertices.size() + m.vertices.size());
    for (size_t i = 0; i < m.vertices.size(); ++i)
    {
        vertices[initial_size + i] = m.vertices[i];
    }

    // Normals
    initial_size = normals.size();
    normals.resize(normals.size() + m.normals.size());
    for (size_t i = 0; i < m.normals.size(); ++i)
    {
        normals[initial_size + i] = m.normals[i];
    }

    // Topology
    int v_shift = vertices.size();
    initial_size = varray.size();
    varray.resize(varray.size() + m.varray.size());
    for (size_t i = 0; i < m.varray.size(); ++i)
    {
        varray[initial_size + i] = m.varray[i] + v_shift;
    }

    int n_shift = normals.size();
    initial_size = narray.size();
    narray.resize(narray.size() + m.narray.size());
    for (size_t i = 0; i < m.narray.size(); ++i)
    {
        narray[initial_size + i] = m.narray[i] + n_shift;
    }
}

void Mesh::SphereWarp(Sphere& sphere)
{
    // TODO:
}

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegularExpression>
#include <QtCore/qstring.h>

/*!
\brief Import a mesh from an .obj file.
\param filename File name.
*/
void Mesh::Load(const QString& filename)
{
  vertices.clear();
  normals.clear();
  varray.clear();
  narray.clear();

  QFile data(filename);

  if (!data.open(QFile::ReadOnly))
    return;
  QTextStream in(&data);

  // Set of regular expressions : Vertex, Normal, Triangle
  QRegularExpression rexv("v\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
  QRegularExpression rexn("vn\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
  QRegularExpression rext("f\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)");
  while (!in.atEnd())
  {
    QString line = in.readLine();
    QRegularExpressionMatch match = rexv.match(line);
    QRegularExpressionMatch matchN = rexn.match(line);
    QRegularExpressionMatch matchT = rext.match(line);
    if (match.hasMatch())//rexv.indexIn(line, 0) > -1)
    {
      Vector q = Vector(match.captured(1).toDouble(), match.captured(2).toDouble(), match.captured(3).toDouble()); vertices.push_back(q);
    }
    else if (matchN.hasMatch())//rexn.indexIn(line, 0) > -1)
    {
      Vector q = Vector(matchN.captured(1).toDouble(), matchN.captured(2).toDouble(), matchN.captured(3).toDouble());  normals.push_back(q);
    }
    else if (matchT.hasMatch())//rext.indexIn(line, 0) > -1)
    {
      varray.push_back(matchT.captured(1).toInt() - 1);
      varray.push_back(matchT.captured(3).toInt() - 1);
      varray.push_back(matchT.captured(5).toInt() - 1);
      narray.push_back(matchT.captured(2).toInt() - 1);
      narray.push_back(matchT.captured(4).toInt() - 1);
      narray.push_back(matchT.captured(6).toInt() - 1);
    }
  }
  data.close();
}

/*!
\brief Save the mesh in .obj format, with vertices and normals.
\param url Filename.
\param meshName %Mesh name in .obj file.
*/
void Mesh::SaveObj(const QString& url, const QString& meshName) const
{
  QFile data(url);
  if (!data.open(QFile::WriteOnly))
    return;
  QTextStream out(&data);
  out << "g " << meshName << Qt::endl;
  for (size_t i = 0; i < vertices.size(); i++)
    out << "v " << vertices.at(i)[0] << " " << vertices.at(i)[1] << " " << vertices.at(i)[2] << QString('\n');
  for (size_t i = 0; i < normals.size(); i++)
    out << "vn " << normals.at(i)[0] << " " << normals.at(i)[1] << " " << normals.at(i)[2] << QString('\n');
  for (size_t i = 0; i < varray.size(); i += 3)
  {
    out << "f " << varray.at(i) + 1 << "//" << narray.at(i) + 1 << " "
      << varray.at(i + 1) + 1 << "//" << narray.at(i + 1) + 1 << " "
      << varray.at(i + 2) + 1 << "//" << narray.at(i + 2) + 1 << " "
      << "\n";
  }
  out.flush();
  data.close();
}

