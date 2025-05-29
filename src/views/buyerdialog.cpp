#include "buyerdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QRegularExpressionValidator>

BuyerDialog::BuyerDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Информация о покупателе"));
    resize(400, 600);

    // Create form layout
    QFormLayout *formLayout = new QFormLayout(this);

    // Create input fields
    lastNameEdit = new QLineEdit(this);
    firstNameEdit = new QLineEdit(this);
    middleNameEdit = new QLineEdit(this);
    passportNumberEdit = new QLineEdit(this);
    phoneNumberEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    propertyTypeCombo = new QComboBox(this);
    propertyAddressEdit = new QLineEdit(this);
    propertyAreaSpinBox = new QDoubleSpinBox(this);
    propertyPriceSpinBox = new QDoubleSpinBox(this);
    purchaseDateEdit = new QDateEdit(this);
    paymentStatusCombo = new QComboBox(this);
    contractNumberEdit = new QLineEdit(this);

    // Setup property type combo
    propertyTypeCombo->addItems({"Квартира", "Дом", "Кондоминиум", "Таунхаус", "Вилла"});

    // Setup validators and masks
    // Passport number mask: "1234 567890"
    passportNumberEdit->setInputMask("9999 999999");
    passportNumberEdit->setPlaceholderText("1234 567890");

    // Phone number mask: "+7 (999) 123-45-67"
    phoneNumberEdit->setInputMask("+7 (999) 999-99-99");
    phoneNumberEdit->setPlaceholderText("+7 (999) 123-45-67");

    // Email validator
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    emailEdit->setValidator(new QRegularExpressionValidator(emailRegex, this));
    emailEdit->setPlaceholderText("example@domain.com");

    // Contract number field (no validation)
    contractNumberEdit->setPlaceholderText("Введите номер договора");
    
    // Setup purchase date edit with proper format
    purchaseDateEdit->setDisplayFormat("yyyy-MM-dd");
    purchaseDateEdit->setCalendarPopup(true);
    purchaseDateEdit->setDate(QDate::currentDate());

    // Setup property area spinbox
    propertyAreaSpinBox->setRange(0.01, 1000);
    propertyAreaSpinBox->setSuffix(" м²");
    propertyAreaSpinBox->setDecimals(2);
    propertyAreaSpinBox->setSingleStep(0.01);

    // Setup property price spinbox
    propertyPriceSpinBox->setRange(0.01, 1000000000);
    propertyPriceSpinBox->setPrefix("$ ");
    propertyPriceSpinBox->setDecimals(2);
    propertyPriceSpinBox->setSingleStep(1000);

    // Setup payment status combo
    paymentStatusCombo->addItems({"В ожидании", "Частичная", "Завершена"});

    // Add fields to form layout
    formLayout->addRow(tr("Фамилия:"), lastNameEdit);
    formLayout->addRow(tr("Имя:"), firstNameEdit);
    formLayout->addRow(tr("Отчество:"), middleNameEdit);
    formLayout->addRow(tr("Номер паспорта:"), passportNumberEdit);
    formLayout->addRow(tr("Номер телефона:"), phoneNumberEdit);
    formLayout->addRow(tr("Email:"), emailEdit);
    formLayout->addRow(tr("Тип недвижимости:"), propertyTypeCombo);
    formLayout->addRow(tr("Адрес недвижимости:"), propertyAddressEdit);
    formLayout->addRow(tr("Площадь недвижимости:"), propertyAreaSpinBox);
    formLayout->addRow(tr("Цена недвижимости:"), propertyPriceSpinBox);
    formLayout->addRow(tr("Дата покупки:"), purchaseDateEdit);
    formLayout->addRow(tr("Статус оплаты:"), paymentStatusCombo);
    formLayout->addRow(tr("Номер договора:"), contractNumberEdit);

    // Add dialog buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    formLayout->addRow(buttonBox);

    // Connect validation signals
    connect(lastNameEdit, &QLineEdit::textChanged, this, &BuyerDialog::validateField);
    connect(firstNameEdit, &QLineEdit::textChanged, this, &BuyerDialog::validateField);
    connect(middleNameEdit, &QLineEdit::textChanged, this, &BuyerDialog::validateField);
    connect(passportNumberEdit, &QLineEdit::textChanged, this, &BuyerDialog::validateField);
    connect(phoneNumberEdit, &QLineEdit::textChanged, this, &BuyerDialog::validateField);
    connect(emailEdit, &QLineEdit::textChanged, this, &BuyerDialog::validateField);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &BuyerDialog::validateAllFields);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void BuyerDialog::validateField()
{
    QLineEdit *sender = qobject_cast<QLineEdit*>(QObject::sender());
    if (!sender) return;

    bool isValid = true;
    if (sender == lastNameEdit || sender == firstNameEdit || sender == middleNameEdit) {
        isValid = !sender->text().trimmed().isEmpty();
    } else if (sender == passportNumberEdit) {
        QRegularExpression regex("^\\d{4}\\s\\d{6}$");
        isValid = regex.match(sender->text()).hasMatch();
    } else if (sender == phoneNumberEdit) {
        QRegularExpression regex("^\\+7\\s\\(\\d{3}\\)\\s\\d{3}-\\d{2}-\\d{2}$");
        isValid = regex.match(sender->text()).hasMatch();
    } else if (sender == emailEdit) {
        QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        isValid = regex.match(sender->text()).hasMatch();
    }
    // Removed contract number validation

    sender->setStyleSheet(isValid ? NORMAL_STYLE : ERROR_STYLE);
}

void BuyerDialog::validateAllFields()
{
    bool allValid = true;

    // Validate required fields
    allValid &= !lastNameEdit->text().trimmed().isEmpty();
    allValid &= !firstNameEdit->text().trimmed().isEmpty();
    allValid &= !middleNameEdit->text().trimmed().isEmpty();

    // Validate formatted fields
    QRegularExpression passportRegex("^\\d{4}\\s\\d{6}$");
    QRegularExpression phoneRegex("^\\+7\\s\\(\\d{3}\\)\\s\\d{3}-\\d{2}-\\d{2}$");
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    allValid &= passportRegex.match(passportNumberEdit->text()).hasMatch();
    allValid &= phoneRegex.match(phoneNumberEdit->text()).hasMatch();
    allValid &= emailRegex.match(emailEdit->text()).hasMatch();

    // Validate numeric fields
    allValid &= (propertyAreaSpinBox->value() > 0);
    allValid &= (propertyPriceSpinBox->value() > 0);

    // Update visual feedback
    lastNameEdit->setStyleSheet(lastNameEdit->text().trimmed().isEmpty() ? ERROR_STYLE : NORMAL_STYLE);
    firstNameEdit->setStyleSheet(firstNameEdit->text().trimmed().isEmpty() ? ERROR_STYLE : NORMAL_STYLE);
    middleNameEdit->setStyleSheet(middleNameEdit->text().trimmed().isEmpty() ? ERROR_STYLE : NORMAL_STYLE);
    passportNumberEdit->setStyleSheet(passportRegex.match(passportNumberEdit->text()).hasMatch() ? NORMAL_STYLE : ERROR_STYLE);
    phoneNumberEdit->setStyleSheet(phoneRegex.match(phoneNumberEdit->text()).hasMatch() ? NORMAL_STYLE : ERROR_STYLE);
    emailEdit->setStyleSheet(emailRegex.match(emailEdit->text()).hasMatch() ? NORMAL_STYLE : ERROR_STYLE);

    if (allValid) {
        accept();
    }
}

void BuyerDialog::setBuyerData(const QString &lastName, const QString &firstName, const QString &middleName,
                             const QString &passportNumber, const QString &phoneNumber, const QString &email,
                             const QString &propertyType, const QString &propertyAddress, double propertyArea,
                             double propertyPrice, const QDate &purchaseDate, const QString &paymentStatus,
                             const QString &contractNumber)
{
    lastNameEdit->setText(lastName);
    firstNameEdit->setText(firstName);
    middleNameEdit->setText(middleName);
    passportNumberEdit->setText(passportNumber);
    phoneNumberEdit->setText(phoneNumber);
    emailEdit->setText(email);
    propertyTypeCombo->setCurrentText(propertyType);
    propertyAddressEdit->setText(propertyAddress);
    propertyAreaSpinBox->setValue(propertyArea);
    propertyPriceSpinBox->setValue(propertyPrice);
    purchaseDateEdit->setDate(purchaseDate);
    paymentStatusCombo->setCurrentText(paymentStatus);
    contractNumberEdit->setText(contractNumber);
}

void BuyerDialog::getBuyerData(QString &lastName, QString &firstName, QString &middleName,
                             QString &passportNumber, QString &phoneNumber, QString &email,
                             QString &propertyType, QString &propertyAddress, double &propertyArea,
                             double &propertyPrice, QDate &purchaseDate, QString &paymentStatus,
                             QString &contractNumber) const
{
    lastName = lastNameEdit->text().trimmed();
    firstName = firstNameEdit->text().trimmed();
    middleName = middleNameEdit->text().trimmed();
    passportNumber = passportNumberEdit->text().trimmed();
    phoneNumber = phoneNumberEdit->text().trimmed();
    email = emailEdit->text().trimmed();
    propertyType = propertyTypeCombo->currentText();
    propertyAddress = propertyAddressEdit->text().trimmed();
    propertyArea = propertyAreaSpinBox->value();
    propertyPrice = propertyPriceSpinBox->value();
    purchaseDate = purchaseDateEdit->date();
    paymentStatus = paymentStatusCombo->currentText();
    contractNumber = contractNumberEdit->text().trimmed();

    // Format the date to match the required format (YYYY-MM-DD)
    QString formattedDate = purchaseDate.toString("yyyy-MM-dd");
    purchaseDate = QDate::fromString(formattedDate, "yyyy-MM-dd");

    // Format the area and price as strings with 2 decimal places
    QString areaStr = QString::number(propertyArea, 'f', 2);
    QString priceStr = QString::number(propertyPrice, 'f', 2);
    propertyArea = areaStr.toDouble();
    propertyPrice = priceStr.toDouble();
} 