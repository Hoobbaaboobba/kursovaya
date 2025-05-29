#ifndef BUYERDIALOG_H
#define BUYERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>

class BuyerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BuyerDialog(QWidget *parent = nullptr);
    void setBuyerData(const QString &lastName, const QString &firstName, const QString &middleName,
                     const QString &passportNumber, const QString &phoneNumber, const QString &email,
                     const QString &propertyType, const QString &propertyAddress, double propertyArea,
                     double propertyPrice, const QDate &purchaseDate, const QString &paymentStatus,
                     const QString &contractNumber);
    void getBuyerData(QString &lastName, QString &firstName, QString &middleName,
                     QString &passportNumber, QString &phoneNumber, QString &email,
                     QString &propertyType, QString &propertyAddress, double &propertyArea,
                     double &propertyPrice, QDate &purchaseDate, QString &paymentStatus,
                     QString &contractNumber) const;

private slots:
    void validateField();
    void validateAllFields();

private:
    void setupValidation();
    void setFieldError(QLineEdit *field, bool hasError);
    bool validateRequiredField(QLineEdit *field);
    bool validatePassportNumber(const QString &text);
    bool validatePhoneNumber(const QString &text);
    bool validateEmail(const QString &text);
    bool validateContractNumber(const QString &text);

    QLineEdit *lastNameEdit;
    QLineEdit *firstNameEdit;
    QLineEdit *middleNameEdit;
    QLineEdit *passportNumberEdit;
    QLineEdit *phoneNumberEdit;
    QLineEdit *emailEdit;
    QComboBox *propertyTypeCombo;
    QLineEdit *propertyAddressEdit;
    QDoubleSpinBox *propertyAreaSpinBox;
    QDoubleSpinBox *propertyPriceSpinBox;
    QDateEdit *purchaseDateEdit;
    QComboBox *paymentStatusCombo;
    QLineEdit *contractNumberEdit;

    const QString ERROR_STYLE = "QLineEdit { border: 2px solid red; background-color: #FFE4E4; }";
    const QString NORMAL_STYLE = "QLineEdit { border: 1px solid gray; }";
};

#endif // BUYERDIALOG_H 