#include "mesh.h"
#include "cylinder.h"
#include "matrix.h"
#include "mathematics.h"
#include <chrono>

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
        vertices.push_back(Vector(disc.Center()[0] + cos(a), disc.Center()[1] + sin(a), disc.Center()[2]));
    }

    // Topology
    for (int i = 1; i < n; i++)
    {
        AddTriangle(i, i+1, 0, 0);
    }
    AddTriangle(0, 1, n, 0);

    // Normals
    SmoothNormals();
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
        vertices.push_back(Vector(cone.Center()[0] + cos(a), cone.Center()[1] + sin(a), cone.Center()[2]));
    }

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

    // Normals
    SmoothNormals();
}

Mesh::Mesh(const Cylinder& c, int n)
{
    // Vertices
    double angle = 2 * M_PI / n;
    vertices.push_back(c.Center()); // Index 0
    for (int i = 0; i < n; ++i)
    {
        double a = angle * i;
        vertices.push_back(Vector(c.Center()[0] + cos(a), c.Center()[1] + sin(a), c.Center()[2]));
    }
    vertices.push_back(c.Center() + c.Normal() * c.Height()); // Index n+1
    for (int i = 0; i < n; ++i)
    {
        double a = angle * i;
        vertices.push_back(Vector(c.Center()[0] + cos(a), c.Center()[1] + sin(a), c.Center()[2] + c.Height()));
    }

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
    AddTriangle(n+2, 1, 2*n+1, 0);

    // Normals
    SmoothNormals();
}

Mesh::Mesh(const Sphere& s,  int n)
{
    // Vertices
    vertices.push_back(s.Center() + s.Normal() * s.Radius());
    for (int i = 0; i < n-1; ++i)
    {
        double phi = M_PI * double(i+1) / double(n);

        for (int j = 0; j < n; ++j)
        {
            double theta = 2.0 * M_PI * double(j) / double(n);
            double x = sin(phi) * cos(theta) * s.Radius();
            double y = sin(phi) * sin(theta) * s.Radius();
            double z = cos(phi) * s.Radius();
            vertices.push_back(Vector(x, y, z) + s.Center());
        }
    }
    vertices.push_back(Vector(s.Center() - s.Normal() * s.Radius()));

    // Topology
    for (int i = 0; i < n; ++i)
    {
        int i0 = i+1;
        int i1 = (i+1) % n +1;
        AddTriangle(0, i1, i0, 0);
        i0 = i + n * (n-2) + 1;
        i1 = (i+1) % n + n * (n-2) + 1;
        AddTriangle(n * (n-1) + 1, i0, i1, 0);
    }

    for (int j = 0; j < n - 2; ++j)
    {
        int j0 = j * n + 1;
        int j1 = (j + 1) * n + 1;
        for (int i = 0; i < n; ++i)
        {
            int i0 = j0 + i;
            int i1 = j0 + (i + 1) % n;
            int i2 = j1 + (i + 1) % n;
            int i3 = j1 + i;
            AddTriangle(i0, i1, i2, 0);
            AddTriangle(i0, i2, i3, 0);
        }
    }

    // Normals
    SmoothNormals();
}

Mesh::Mesh(const Tore& t, int n)
{
    // Vertices
    for (int i = 0; i < n; ++i)
    {
        double phi = 2 * M_PI * double(i) / double(n);

        for (int j = 0; j < n; ++j)
        {
            double theta = 2.0 * M_PI * double(j) / double(n);
            double x = sin(phi) * t.Radius() + sin(phi) * sin(theta) * t.radius();
            double y = cos(phi) * t.Radius() + cos(phi) * sin(theta) * t.radius();
            double z = cos(theta) * t.radius();
            vertices.push_back(Vector(x, y, z) + t.Center());
        }
    }

    // Topology
    for (int i = 0; i < n; ++i)
    {
        int i0 = (i * n) % (n * n);
        int i1 = ((i + 1) * n) % (n * n);
        for (int j = 0; j < n; ++j)
        {
            int j0 = i0 + j;
            int j1 = i1 + j;
            int j2 = i1 + (j + 1) % n;
            int j3 = i0 + (j + 1) % n;
            AddTriangle(j0, j1, j2, 0);
            AddTriangle(j0, j2, j3, 0);
        }
    }

    // Normals
    SmoothNormals();
}

Mesh::Mesh(const Capsule& c, int n)
{
    if (n % 2 == 0) n -= 1;

    // Vertices
    vertices.push_back(c.Center() + c.Normal() * (c.Radius() + c.Height()));
    for (int i = 0; i < n - 1; ++i)
    {
        double phi = M_PI * double(i+1) / double(n);

        for (int j = 0; j < n; ++j)
        {
            double theta = 2.0 * M_PI * double(j) / double(n);
            double x = sin(phi) * cos(theta) * c.Radius();
            double y = sin(phi) * sin(theta) * c.Radius();
            double z = cos(phi) * c.Radius();
            if (i <= n / 2 - 1) {
                vertices.push_back(Vector(x, y, z) + c.Center() + c.Normal() * c.Height());
            } else {
                vertices.push_back(Vector(x, y, z) + c.Center());
            }
        }
    }
    vertices.push_back(c.Center() - c.Normal() * c.Radius());

    // Topology
    for (int i = 0; i < n; ++i)
    {
        int i0 = i+1;
        int i1 = (i+1) % n +1;
        AddTriangle(0, i1, i0, 0);
        i0 = i + n * (n-2) + 1;
        i1 = (i+1) % n + n * (n-2) + 1;
        AddTriangle(n * (n-1) + 1, i0, i1, 0);
    }

    for (int j = 0; j < n - 2; ++j)
    {
        int j0 = j * n + 1;
        int j1 = (j + 1) * n + 1;
        for (int i = 0; i < n; ++i)
        {
            int i0 = j0 + i;
            int i1 = j0 + (i + 1) % n;
            int i2 = j1 + (i + 1) % n;
            int i3 = j1 + i;
            AddTriangle(i0, i1, i2, 0);
            AddTriangle(i0, i2, i3, 0);
        }
    }

    // Normals
    SmoothNormals();
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

void Mesh::Translate(const Vector& t)
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
        vertices[i] = m * vertices[i];
    }

    // Normals
    for (size_t i = 0; i < normals.size(); ++i)
    {
        normals[i] = m * normals[i];
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

void Mesh::Merge(const Mesh& m)
{
    // initializing offsets before adding verices and normals
    int v_off = vertices.size();
    int n_off = normals.size();

    // adding m's vertices
    for (int i = 0; i < m.Vertexes(); ++i)
    {
        vertices.push_back(m.Vertex(i));
    }

    // adding m's normals
    for (int i = 0; i < m.Triangles()*3; ++i)
    {
        normals.push_back(m.Normal(i));
    }

    for (int i = 0; i < m.Triangles(); ++i)
    {
        AddSmoothTriangle(
            m.VertexIndex(i, 0) + v_off,
            m.NormalIndex(i, 0) + n_off,
            m.VertexIndex(i, 1) + v_off,
            m.NormalIndex(i, 1) + n_off,
            m.VertexIndex(i, 2) + v_off,
            m.NormalIndex(i, 2) + n_off
        );
    }
}

void Mesh::SphereWarp(const Sphere& s, double force)
{
    for (int i = 0; i < Vertexes(); ++i)
    {
        if (s.Contains(vertices[i]))
        {
            Vector dir(s.Center() - vertices[i]);
            double dist = Norm(dir);
            dir += s.Center() * abs(force);
            Normalize(dir);
            // double distanceMul = (- 1 * sqrt(dist) + sqrt(s.Radius()));
            double distanceMul = - 3 * pow(dist/s.Radius(), 2) + 2 * pow(dist/s.Radius(), 3) + 1;
            vertices[i] += dir * force * distanceMul;
        }
    }

    SmoothNormals();
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

