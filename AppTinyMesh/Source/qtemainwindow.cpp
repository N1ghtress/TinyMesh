#include "qte.h"
#include "implicits.h"
#include "ui_interface.h"

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;
	QGridLayout* GLlayout = new QGridLayout;
	GLlayout->addWidget(meshWidget, 0, 0);
	GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

	// Creation des connect
	CreateActions();

	meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
	delete meshWidget;
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));

    // Added
    connect(uiw->discMesh, SIGNAL(clicked()), this, SLOT(DiscMeshExample()));
    connect(uiw->coneMesh, SIGNAL(clicked()), this, SLOT(ConeMeshExample()));
    connect(uiw->cylinderMesh, SIGNAL(clicked()), this, SLOT(CylinderMeshExample()));
    connect(uiw->sphereMesh, SIGNAL(clicked()), this, SLOT(SphereMeshExample()));
    connect(uiw->toreMesh, SIGNAL(clicked()), this, SLOT(ToreMeshExample()));
    connect(uiw->capsuleMesh, SIGNAL(clicked()), this, SLOT(CapsuleMeshExample()));
    connect(uiw->complexMesh, SIGNAL(clicked()), this, SLOT(ComplexMeshExample()));

    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

	// Widget edition
	connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
	connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
}

void MainWindow::editingSceneLeft(const Ray&)
{
}

void MainWindow::editingSceneRight(const Ray&)
{
}

void MainWindow::BoxMeshExample()
{
    Mesh mesh = Mesh(Box(1.0));

    mesh.RotateX(M_PI / 4);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

// Added
void MainWindow::DiscMeshExample()
{
    Mesh mesh = Mesh(Disc(2.0), 25);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

void MainWindow::ConeMeshExample()
{
    Mesh mesh = Mesh(Cone(1.0, 3.0), 25);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

void MainWindow::CylinderMeshExample()
{
    Mesh mesh = Mesh(Cylinder(), 100);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

void MainWindow::SphereMeshExample()
{
    Mesh mesh = Mesh(Sphere(), 100);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

void MainWindow::ToreMeshExample()
{
    Mesh mesh = Mesh(Tore(), 100);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

void MainWindow::CapsuleMeshExample()
{
    Mesh mesh = Mesh(Capsule(), 100);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

void MainWindow::ComplexMeshExample()
{
    Mesh mesh = Mesh(Box(1.0));
    Vector v(0.0, 0.0, 1.1);
    Mesh s_mesh = Mesh(Sphere(v, 1.0), 100);

    mesh.Merge(s_mesh);

    meshColor = MeshColor(mesh);
    UpdateGeometry();
}

void MainWindow::SphereImplicitExample()
{
  AnalyticScalarField implicit;

  Mesh implicitMesh;
  implicit.Polygonize(31, implicitMesh, Box(2.0));

  std::vector<Color> cols;
  cols.resize(implicitMesh.Vertexes());
  for (size_t i = 0; i < cols.size(); i++)
    cols[i] = Color(0.8, 0.8, 0.8);

  meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
  UpdateGeometry();
}

void MainWindow::UpdateGeometry()
{
	meshWidget->ClearAll();
	meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));
    uiw->lineEdit_gen_time->setText(QString::number(meshColor.GenTime()));

	UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
		meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
	else
		meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
	meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}
