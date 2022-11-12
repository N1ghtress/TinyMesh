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
    auto start = std::chrono::high_resolution_clock::now();

    Mesh mesh = Mesh(Box(1.0));

    mesh.RotateX(M_PI / 4);

    meshColor = MeshColor(mesh);

    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

    UpdateGeometry();
}

// Added
void MainWindow::DiscMeshExample()
{
    auto start = std::chrono::high_resolution_clock::now();

    Mesh mesh = Mesh(Disc(2.0), 25);

    meshColor = MeshColor(mesh);

    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

    UpdateGeometry();
}

void MainWindow::ConeMeshExample()
{
    auto start = std::chrono::high_resolution_clock::now();

    Mesh mesh = Mesh(Cone(1.0, 3.0), 25);

    meshColor = MeshColor(mesh);

    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

    UpdateGeometry();
}

void MainWindow::CylinderMeshExample()
{
    auto start = std::chrono::high_resolution_clock::now();

    Mesh mesh = Mesh(Cylinder(), 1000);

    meshColor = MeshColor(mesh);

    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

    UpdateGeometry();
}

void MainWindow::SphereMeshExample()
{
    auto start = std::chrono::high_resolution_clock::now();

    Mesh mesh = Mesh(Sphere(), 1000);

    meshColor = MeshColor(mesh);

    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

    UpdateGeometry();
}

void MainWindow::ToreMeshExample()
{
    auto start = std::chrono::high_resolution_clock::now();

    Mesh mesh = Mesh(Tore(), 1000);

    meshColor = MeshColor(mesh);

    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

    UpdateGeometry();
}

void MainWindow::CapsuleMeshExample()
{
    auto start = std::chrono::high_resolution_clock::now();

    Mesh mesh = Mesh(Capsule(), 1000);

    meshColor = MeshColor(mesh);

    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

    UpdateGeometry();
}

void MainWindow::ComplexMeshExample()
{
    // Time measure
    auto start = std::chrono::high_resolution_clock::now();

    // Mesh creation and transformation
    Mesh mesh;
    Mesh dice0 = Mesh(Sphere(Vector(0, 0, 0), 1), 100);

    double r = 1.4;
    double f = -2;

    dice0.SphereWarp(Sphere(Vector(2, 0, 0), r), f);
    dice0.SphereWarp(Sphere(Vector(-2, 0, 0), r), f);
    dice0.SphereWarp(Sphere(Vector(0, 2, 0), r), f);
    dice0.SphereWarp(Sphere(Vector(0, -2, 0), r), f);
    dice0.SphereWarp(Sphere(Vector(0, 0, 2), r), f);
    dice0.SphereWarp(Sphere(Vector(0, 0, -2), r), f);

    Mesh dice1 = Mesh(dice0);
    Mesh dice2 = Mesh(dice0);

    dice0.Translate(Vector(0, -0.7, 0));

    dice1.RotateZ(M_PI / 4);
    dice1.Translate(Vector(1, 1, 0));

    dice2.RotateX(M_PI / 4);
    dice2.Translate(Vector(-1, 1, 0));


    // Mesh assembly
    mesh.Merge(dice0);
    mesh.Merge(dice1);
    mesh.Merge(dice2);

    meshColor = MeshColor(mesh);

    // Time measure
    auto stop = std::chrono::high_resolution_clock::now();
    double gen_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    uiw->lineEdit_gen_time->setText(QString::number(gen_time));

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
