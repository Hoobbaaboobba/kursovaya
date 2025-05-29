#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QValidator>
#include "../models/studentmodel.h"

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudentDialog(QWidget *parent = nullptr);
    StudentDialog(const Student &student, QWidget *parent = nullptr);
    Student getStudent() const;

private:
    void setupUI();
    void setupValidators();
    void setupConnections();
    void loadStudent(const Student &student);

    QLineEdit *lastNameEdit;
    QLineEdit *firstNameEdit;
    QLineEdit *middleNameEdit;
    QLineEdit *groupEdit;
    QComboBox *courseCombo;
    QLineEdit *instituteEdit;
    QLineEdit *specializationEdit;
    QLineEdit *averageGradeEdit;
    QLineEdit *studentIdEdit;
    QLineEdit *enrollmentYearEdit;
    QLineEdit *graduationYearEdit;
    QComboBox *statusCombo;

    QPushButton *okButton;
    QPushButton *cancelButton;

    Student currentStudent;
};

#endif // STUDENTDIALOG_H 