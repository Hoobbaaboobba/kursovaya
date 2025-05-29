#include "studentdialog.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

StudentDialog::StudentDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setupValidators();
    setupConnections();
}

StudentDialog::StudentDialog(const Student &student, QWidget *parent)
    : QDialog(parent)
    , currentStudent(student)
{
    setupUI();
    setupValidators();
    setupConnections();
    loadStudent(student);
}

void StudentDialog::setupUI()
{
    setWindowTitle(tr("Student Information"));
    setMinimumWidth(400);

    QFormLayout *layout = new QFormLayout(this);

    // Create input fields
    lastNameEdit = new QLineEdit(this);
    firstNameEdit = new QLineEdit(this);
    middleNameEdit = new QLineEdit(this);
    groupEdit = new QLineEdit(this);
    courseCombo = new QComboBox(this);
    instituteEdit = new QLineEdit(this);
    specializationEdit = new QLineEdit(this);
    averageGradeEdit = new QLineEdit(this);
    studentIdEdit = new QLineEdit(this);
    enrollmentYearEdit = new QLineEdit(this);
    graduationYearEdit = new QLineEdit(this);
    statusCombo = new QComboBox(this);

    // Add course options
    for (int i = 1; i <= 6; ++i) {
        courseCombo->addItem(QString::number(i));
    }

    // Add status options
    statusCombo->addItems({tr("Active"), tr("Graduated"), tr("On Leave"), tr("Expelled")});

    // Add fields to layout
    layout->addRow(tr("Last Name:"), lastNameEdit);
    layout->addRow(tr("First Name:"), firstNameEdit);
    layout->addRow(tr("Middle Name:"), middleNameEdit);
    layout->addRow(tr("Group:"), groupEdit);
    layout->addRow(tr("Course:"), courseCombo);
    layout->addRow(tr("Institute:"), instituteEdit);
    layout->addRow(tr("Specialization:"), specializationEdit);
    layout->addRow(tr("Average Grade:"), averageGradeEdit);
    layout->addRow(tr("Student ID:"), studentIdEdit);
    layout->addRow(tr("Enrollment Year:"), enrollmentYearEdit);
    layout->addRow(tr("Graduation Year:"), graduationYearEdit);
    layout->addRow(tr("Status:"), statusCombo);

    // Add buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    okButton = new QPushButton(tr("OK"), this);
    cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addRow(buttonLayout);

    setLayout(layout);
}

void StudentDialog::setupValidators()
{
    // Group format: XX-XXX (e.g., CS-101)
    QRegularExpression groupRegex("^[A-Z]{2}-\\d{3}$");
    groupEdit->setValidator(new QRegularExpressionValidator(groupRegex, this));

    // Student ID format: YYYYXX### (e.g., 2023CS001)
    QRegularExpression idRegex("^\\d{4}[A-Z]{2}\\d{3}$");
    studentIdEdit->setValidator(new QRegularExpressionValidator(idRegex, this));

    // Year format: YYYY
    QRegularExpression yearRegex("^\\d{4}$");
    enrollmentYearEdit->setValidator(new QRegularExpressionValidator(yearRegex, this));
    graduationYearEdit->setValidator(new QRegularExpressionValidator(yearRegex, this));

    // Average grade: 0-100
    QRegularExpression gradeRegex("^\\d{1,2}(\\.\\d{1,2})?$");
    averageGradeEdit->setValidator(new QRegularExpressionValidator(gradeRegex, this));
}

void StudentDialog::setupConnections()
{
    connect(okButton, &QPushButton::clicked, this, &StudentDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &StudentDialog::reject);
}

void StudentDialog::loadStudent(const Student &student)
{
    lastNameEdit->setText(student.lastName);
    firstNameEdit->setText(student.firstName);
    middleNameEdit->setText(student.middleName);
    groupEdit->setText(student.group);
    courseCombo->setCurrentText(student.course);
    instituteEdit->setText(student.institute);
    specializationEdit->setText(student.specialization);
    averageGradeEdit->setText(student.averageGrade);
    studentIdEdit->setText(student.studentId);
    enrollmentYearEdit->setText(student.enrollmentYear);
    graduationYearEdit->setText(student.graduationYear);
    statusCombo->setCurrentText(student.status);
}

Student StudentDialog::getStudent() const
{
    Student student;
    student.lastName = lastNameEdit->text();
    student.firstName = firstNameEdit->text();
    student.middleName = middleNameEdit->text();
    student.group = groupEdit->text();
    student.course = courseCombo->currentText();
    student.institute = instituteEdit->text();
    student.specialization = specializationEdit->text();
    student.averageGrade = averageGradeEdit->text();
    student.studentId = studentIdEdit->text();
    student.enrollmentYear = enrollmentYearEdit->text();
    student.graduationYear = graduationYearEdit->text();
    student.status = statusCombo->currentText();
    return student;
} 