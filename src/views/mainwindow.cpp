void MainWindow::addBuyer()
{
    BuyerDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString lastName, firstName, middleName, passportNumber, phoneNumber, email,
                propertyType, propertyAddress, paymentStatus, contractNumber;
        double propertyArea, propertyPrice;
        QDate purchaseDate;
        
        dialog.getBuyerData(lastName, firstName, middleName, passportNumber, phoneNumber, email,
                          propertyType, propertyAddress, propertyArea, propertyPrice,
                          purchaseDate, paymentStatus, contractNumber);

        // Debug output to verify the data
        qDebug() << "Adding buyer with data:";
        qDebug() << "Name:" << lastName << firstName << middleName;
        qDebug() << "Contact:" << passportNumber << phoneNumber << email;
        qDebug() << "Property:" << propertyType << propertyAddress;
        qDebug() << "Area:" << propertyArea << "Price:" << propertyPrice;
        qDebug() << "Date:" << purchaseDate.toString("yyyy-MM-dd");
        qDebug() << "Status:" << paymentStatus << "Contract:" << contractNumber;

        Buyer buyer;
        buyer.lastName = lastName.trimmed();
        buyer.firstName = firstName.trimmed();
        buyer.middleName = middleName.trimmed();
        buyer.passportNumber = passportNumber.trimmed();
        buyer.phoneNumber = phoneNumber.trimmed();
        buyer.email = email.trimmed();
        buyer.propertyType = propertyType.trimmed();
        buyer.propertyAddress = propertyAddress.trimmed();
        buyer.propertyArea = QString::number(propertyArea, 'f', 1);  // Format with 1 decimal place
        buyer.propertyPrice = QString::number(propertyPrice, 'f', 0); // Format as whole number
        buyer.purchaseDate = purchaseDate.toString("yyyy-MM-dd");
        buyer.paymentStatus = paymentStatus.trimmed();
        buyer.contractNumber = contractNumber.trimmed();

        // Debug output to verify the formatted data
        qDebug() << "Formatted buyer data:";
        qDebug() << "Name:" << buyer.lastName << buyer.firstName << buyer.middleName;
        qDebug() << "Contact:" << buyer.passportNumber << buyer.phoneNumber << buyer.email;
        qDebug() << "Property:" << buyer.propertyType << buyer.propertyAddress;
        qDebug() << "Area:" << buyer.propertyArea << "Price:" << buyer.propertyPrice;
        qDebug() << "Date:" << buyer.purchaseDate;
        qDebug() << "Status:" << buyer.paymentStatus << "Contract:" << buyer.contractNumber;

        if (model->addBuyer(buyer)) {
            statusBar()->showMessage(tr("Покупатель успешно добавлен"), 3000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось добавить покупателя"));
        }
    }
}

void MainWindow::editBuyer()
{
    QModelIndex currentIndex = tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Выберите покупателя для редактирования"));
        return;
    }

    Buyer currentBuyer = model->getBuyer(currentIndex.row());
    BuyerDialog dialog(this);
    
    // Convert string values to double for the dialog
    double area = currentBuyer.propertyArea.toDouble();
    double price = currentBuyer.propertyPrice.toDouble();
    QDate date = QDate::fromString(currentBuyer.purchaseDate, "yyyy-MM-dd");
    
    dialog.setBuyerData(currentBuyer.lastName, currentBuyer.firstName, currentBuyer.middleName,
                       currentBuyer.passportNumber, currentBuyer.phoneNumber, currentBuyer.email,
                       currentBuyer.propertyType, currentBuyer.propertyAddress, area, price,
                       date, currentBuyer.paymentStatus, currentBuyer.contractNumber);

    if (dialog.exec() == QDialog::Accepted) {
        QString lastName, firstName, middleName, passportNumber, phoneNumber, email,
                propertyType, propertyAddress, paymentStatus, contractNumber;
        double propertyArea, propertyPrice;
        QDate purchaseDate;
        
        dialog.getBuyerData(lastName, firstName, middleName, passportNumber, phoneNumber, email,
                          propertyType, propertyAddress, propertyArea, propertyPrice,
                          purchaseDate, paymentStatus, contractNumber);

        // Debug output to verify the data
        qDebug() << "Updating buyer with data:";
        qDebug() << "Name:" << lastName << firstName << middleName;
        qDebug() << "Contact:" << passportNumber << phoneNumber << email;
        qDebug() << "Property:" << propertyType << propertyAddress;
        qDebug() << "Area:" << propertyArea << "Price:" << propertyPrice;
        qDebug() << "Date:" << purchaseDate.toString("yyyy-MM-dd");
        qDebug() << "Status:" << paymentStatus << "Contract:" << contractNumber;

        Buyer updatedBuyer;
        updatedBuyer.lastName = lastName.trimmed();
        updatedBuyer.firstName = firstName.trimmed();
        updatedBuyer.middleName = middleName.trimmed();
        updatedBuyer.passportNumber = passportNumber.trimmed();
        updatedBuyer.phoneNumber = phoneNumber.trimmed();
        updatedBuyer.email = email.trimmed();
        updatedBuyer.propertyType = propertyType.trimmed();
        updatedBuyer.propertyAddress = propertyAddress.trimmed();
        updatedBuyer.propertyArea = QString::number(propertyArea, 'f', 1);  // Format with 1 decimal place
        updatedBuyer.propertyPrice = QString::number(propertyPrice, 'f', 0); // Format as whole number
        updatedBuyer.purchaseDate = purchaseDate.toString("yyyy-MM-dd");
        updatedBuyer.paymentStatus = paymentStatus.trimmed();
        updatedBuyer.contractNumber = contractNumber.trimmed();

        // Debug output to verify the formatted data
        qDebug() << "Formatted updated buyer data:";
        qDebug() << "Name:" << updatedBuyer.lastName << updatedBuyer.firstName << updatedBuyer.middleName;
        qDebug() << "Contact:" << updatedBuyer.passportNumber << updatedBuyer.phoneNumber << updatedBuyer.email;
        qDebug() << "Property:" << updatedBuyer.propertyType << updatedBuyer.propertyAddress;
        qDebug() << "Area:" << updatedBuyer.propertyArea << "Price:" << updatedBuyer.propertyPrice;
        qDebug() << "Date:" << updatedBuyer.purchaseDate;
        qDebug() << "Status:" << updatedBuyer.paymentStatus << "Contract:" << updatedBuyer.contractNumber;

        if (model->updateBuyer(currentIndex.row(), updatedBuyer)) {
            statusBar()->showMessage(tr("Покупатель успешно обновлен"), 3000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось обновить данные покупателя"));
        }
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить файл"), "",
        tr("Текстовые файлы (*.txt);;Все файлы (*)"));

    if (fileName.isEmpty()) {
        qDebug() << "Save operation cancelled by user";
        return;
    }

    qDebug() << "Attempting to save file to:" << fileName;

    if (!model->saveToFile(fileName)) {
        qDebug() << "Failed to save file:" << fileName;
        QMessageBox::warning(this, tr("Ошибка"),
            tr("Не удалось сохранить файл: %1").arg(fileName));
        return;
    }

    qDebug() << "File saved successfully:" << fileName;
    statusBar()->showMessage(tr("Файл успешно сохранен"), 3000);
} 