
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QGroupBox>
#include <vector>
#include <cmath>

struct SpaceObject {
    QString id;
    double x, y;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("ASTRA - Space Traffic Monitoring");
    window.resize(800, 600);

    // Dark navy theme
    window.setStyleSheet(R"(
        QWidget {
            background: #171E2B;
            color: #E5EAF2;
            font-family: "Segoe UI";
            font-size: 10pt;
        }

        QLabel#title {
            color: #62A8E8;
            font-size: 27pt;
            font-weight: bold;
        }

        QLabel#subtitle {
            color: #9BAFC5;
        }

        QGroupBox {
            background: #222D3D;
            border: 1px solid #35465B;
            border-radius: 9px;
            margin-top: 12px;
            padding: 14px;
            font-weight: bold;
            color: #80B9EC;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 5px;
        }

        QLineEdit {
            background: #303D50;
            color: white;
            border: 1px solid #465A72;
            border-radius: 6px;
            padding: 9px;
        }

        QLineEdit:focus {
            border: 1px solid #62A8E8;
        }

        QPushButton {
            border: none;
            border-radius: 6px;
            padding: 10px;
            font-weight: bold;
        }

        QPushButton#add {
            background: #2878B9;
            color: white;
        }

        QPushButton#check {
            background: #16836B;
            color: white;
        }

        QPushButton#clear {
            background: #8D4248;
            color: white;
        }

        QPushButton:hover {
            border: 1px solid #A8C9E8;
        }

        QTableWidget {
            background: #222D3D;
            alternate-background-color: #293649;
            color: #E5EAF2;
            border: 1px solid #35465B;
            gridline-color: #35465B;
            selection-background-color: #365B7E;
        }

        QHeaderView::section {
            background: #303F53;
            color: #A9D0F2;
            padding: 10px;
            border: none;
            font-weight: bold;
        }

        QLabel#status {
            background: #26374B;
            color: #A9C9E8;
            padding: 10px;
            border-radius: 6px;
        }
    )");

    // Heading
    QLabel *title = new QLabel("ASTRA");
    title->setObjectName("title");

    QLabel *subtitle = new QLabel(
        "Adaptive Space Traffic and Risk Avoidance System");
    subtitle->setObjectName("subtitle");

    // Input fields
    QLineEdit *idInput = new QLineEdit;
    idInput->setPlaceholderText("Object ID (SAT-01)");

    QLineEdit *xInput = new QLineEdit;
    xInput->setPlaceholderText("X Position");

    QLineEdit *yInput = new QLineEdit;
    yInput->setPlaceholderText("Y Position");

    // Buttons
    QPushButton *addButton = new QPushButton("Add Object");
    addButton->setObjectName("add");

    QPushButton *checkButton =
        new QPushButton("Check Collision Risk");
    checkButton->setObjectName("check");

    QPushButton *clearButton = new QPushButton("Clear All");
    clearButton->setObjectName("clear");

    // Object table
    QTableWidget *table = new QTableWidget(0, 3);
    table->setHorizontalHeaderLabels(
        {"Object ID", "X Position", "Y Position"});

    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    table->verticalHeader()->setVisible(false);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Status
    QLabel *status = new QLabel("System ready. Add space objects.");
    status->setObjectName("status");

    // Store objects
    std::vector<SpaceObject> objects;

    // Input section
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(idInput, 2);
    inputLayout->addWidget(xInput);
    inputLayout->addWidget(yInput);

    QGroupBox *inputGroup = new QGroupBox("Add Space Object");
    QVBoxLayout *inputBox = new QVBoxLayout;
    inputBox->addLayout(inputLayout);
    inputBox->addWidget(addButton);
    inputGroup->setLayout(inputBox);

    // Table section
    QGroupBox *tableGroup = new QGroupBox("Tracked Space Objects");
    QVBoxLayout *tableBox = new QVBoxLayout;
    tableBox->addWidget(table);
    tableGroup->setLayout(tableBox);

    // Add object
    QObject::connect(addButton, &QPushButton::clicked, [&]() {
        QString id = idInput->text().trimmed();

        bool validX, validY;
        double x = xInput->text().toDouble(&validX);
        double y = yInput->text().toDouble(&validY);

        if (id.isEmpty() || !validX || !validY) {
            QMessageBox::warning(&window, "Invalid Input",
                                 "Enter a valid ID and numeric coordinates.");
            return;
        }

        for (const auto &obj : objects) {
            if (obj.id == id) {
                QMessageBox::warning(&window, "Duplicate ID",
                                     "This object ID already exists.");
                return;
            }
        }

        objects.push_back({id, x, y});

        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(id));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(x)));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(y)));

        idInput->clear();
        xInput->clear();
        yInput->clear();

        status->setText("Object added. Total objects: "
                        + QString::number(objects.size()));
    });

    // Collision detection
    QObject::connect(checkButton, &QPushButton::clicked, [&]() {
        if (objects.size() < 2) {
            QMessageBox::information(&window, "Information",
                                     "Add at least two objects first.");
            return;
        }

        const double threshold = 10.0;
        QString report;
        int riskyPairs = 0;

        for (size_t i = 0; i < objects.size(); i++) {
            for (size_t j = i + 1; j < objects.size(); j++) {

                double dx = objects[i].x - objects[j].x;
                double dy = objects[i].y - objects[j].y;
                double distance = sqrt(dx * dx + dy * dy);

                if (distance < threshold) {
                    riskyPairs++;
                    report += objects[i].id + " and "
                              + objects[j].id + " are too close. Distance: "
                              + QString::number(distance, 'f', 2) + "\n";
                }
            }
        }

        if (riskyPairs == 0) {
            status->setText("No risky pairs found.");
            QMessageBox::information(&window, "Collision Check",
                                     "No risky pairs found.");
        }
        else {
            status->setText("Warning! "
                            + QString::number(riskyPairs) + " risky pair(s) found.");
            QMessageBox::warning(&window, "Collision Warning", report);
        }
    });

    // Clear all objects
    QObject::connect(clearButton, &QPushButton::clicked, [&]() {
        objects.clear();
        table->setRowCount(0);
        status->setText("All objects cleared.");
    });

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(22, 20, 22, 20);
    mainLayout->setSpacing(12);

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(tableGroup, 1);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(checkButton);
    buttons->addWidget(clearButton);

    mainLayout->addLayout(buttons);
    mainLayout->addWidget(status);

    window.setLayout(mainLayout);
    window.show();

    return app.exec();
}
