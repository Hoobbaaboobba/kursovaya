#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "views/buyerdialog.h"
#include "dialogs/aboutdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QChart>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new BuyerModel(this))
    , controller(new BuyerController(model, this))
    , view(new BuyerView(this))
{
    ui->setupUi(this);
    setCentralWidget(view);
    view->setModel(model);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
    setupConnections();
    setupContextMenu();
    setupDragAndDrop();

    loadSettings();
    updateWindowTitle();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::createActions()
{
    // File actions
    ui->actionOpen->setShortcut(QKeySequence::Open);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);

    ui->actionSave->setShortcut(QKeySequence::Save);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);

    ui->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);

    ui->actionExit->setShortcut(QKeySequence::Quit);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);

    // Edit actions
    ui->actionAddBuyer->setShortcut(QKeySequence::New);
    connect(ui->actionAddBuyer, &QAction::triggered, this, &MainWindow::addBuyer);

    ui->actionEditBuyer->setShortcut(QKeySequence("Ctrl+E"));
    connect(ui->actionEditBuyer, &QAction::triggered, this, &MainWindow::editBuyer);

    ui->actionDeleteBuyer->setShortcut(QKeySequence::Delete);
    connect(ui->actionDeleteBuyer, &QAction::triggered, this, &MainWindow::deleteBuyer);

    // View actions
    connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::searchBuyers);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::printView);
    connect(ui->actionShowChart, &QAction::triggered, this, &MainWindow::showChart);

    // Language actions
    connect(ui->actionEnglish, &QAction::triggered, [this]() { changeLanguage(); });
    connect(ui->actionRussian, &QAction::triggered, [this]() { changeLanguage(); });

    // Help actions
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    // File menu
    ui->menuFile->addAction(ui->actionOpen);
    ui->menuFile->addAction(ui->actionSave);
    ui->menuFile->addAction(ui->actionSaveAs);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(ui->actionExit);

    // Edit menu
    ui->menuEdit->addAction(ui->actionAddBuyer);
    ui->menuEdit->addAction(ui->actionEditBuyer);
    ui->menuEdit->addAction(ui->actionDeleteBuyer);

    // View menu
    ui->menuView->addAction(ui->actionSearch);
    ui->menuView->addAction(ui->actionPrint);
    ui->menuView->addAction(ui->actionShowChart);

    // Language menu
    ui->menuLanguage->addAction(ui->actionEnglish);
    ui->menuLanguage->addAction(ui->actionRussian);

    // Help menu
    ui->menuHelp->addAction(ui->actionAbout);
}

void MainWindow::createToolBars()
{
    // File toolbar
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addSeparator();

    // Edit toolbar
    ui->mainToolBar->addAction(ui->actionAddBuyer);
    ui->mainToolBar->addAction(ui->actionEditBuyer);
    ui->mainToolBar->addAction(ui->actionDeleteBuyer);
    ui->mainToolBar->addSeparator();

    // View toolbar
    ui->mainToolBar->addAction(ui->actionSearch);
    ui->mainToolBar->addAction(ui->actionPrint);
    ui->mainToolBar->addAction(ui->actionShowChart);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готов"));
}

void MainWindow::createDockWindows()
{
    // Add any dock windows here if needed
}

void MainWindow::setupConnections()
{
    connect(view, &BuyerView::addBuyerRequested, this, &MainWindow::addBuyer);
    connect(view, &BuyerView::editBuyerRequested, this, &MainWindow::editBuyer);
    connect(view, &BuyerView::deleteBuyerRequested, this, &MainWindow::deleteBuyer);
    connect(view, &BuyerView::searchRequested, this, &MainWindow::searchBuyers);
    connect(view, &BuyerView::printRequested, this, &MainWindow::printView);
    connect(view, &BuyerView::showChartRequested, this, &MainWindow::showChart);

    connect(controller, &BuyerController::fileOpened, this, &MainWindow::updateWindowTitle);
    connect(controller, &BuyerController::fileSaved, this, &MainWindow::updateWindowTitle);
    connect(controller, &BuyerController::error, this, [this](const QString &message) {
        QMessageBox::warning(this, tr("Ошибка"), message);
    });
}

void MainWindow::setupContextMenu()
{
    view->setupContextMenu();
}

void MainWindow::setupDragAndDrop()
{
    view->setupDragAndDrop();
}

void MainWindow::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    currentFile = settings.value("currentFile").toString();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("currentFile", currentFile);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл"), QString(),
        tr("Текстовые файлы (*.txt);;Все файлы (*)"));

    if (!fileName.isEmpty()) {
        if (controller->openFile(fileName)) {
            currentFile = fileName;
            updateWindowTitle();
            statusBar()->showMessage(tr("Файл загружен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось открыть файл %1").arg(fileName));
        }
    }
}

void MainWindow::saveFile()
{
    if (currentFile.isEmpty()) {
        saveFileAs();
    } else {
        if (controller->saveFile(currentFile)) {
            statusBar()->showMessage(tr("Файл сохранен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось сохранить файл %1").arg(currentFile));
        }
    }
}

void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить файл"), QString(),
        tr("Текстовые файлы (*.txt);;Все файлы (*)"));

    if (!fileName.isEmpty()) {
        if (controller->saveFile(fileName)) {
            currentFile = fileName;
            updateWindowTitle();
            statusBar()->showMessage(tr("Файл сохранен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось сохранить файл %1").arg(fileName));
        }
    }
}

void MainWindow::addBuyer()
{
    BuyerDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString lastName, firstName, middleName;
        QString passportNumber, phoneNumber, email;
        QString propertyType, propertyAddress;
        double propertyArea, propertyPrice;
        QDate purchaseDate;
        QString paymentStatus, contractNumber;

        dialog.getBuyerData(lastName, firstName, middleName,
                          passportNumber, phoneNumber, email,
                          propertyType, propertyAddress, propertyArea,
                          propertyPrice, purchaseDate, paymentStatus,
                          contractNumber);

        Buyer buyer;
        buyer.lastName = lastName;
        buyer.firstName = firstName;
        buyer.middleName = middleName;
        buyer.passportNumber = passportNumber;
        buyer.phoneNumber = phoneNumber;
        buyer.email = email;
        buyer.propertyType = propertyType;
        buyer.propertyAddress = propertyAddress;
        buyer.propertyArea = QString::number(propertyArea);
        buyer.propertyPrice = QString::number(propertyPrice);
        buyer.purchaseDate = purchaseDate.toString("yyyy-MM-dd");
        buyer.paymentStatus = paymentStatus;
        buyer.contractNumber = contractNumber;

        if (controller->addBuyer(buyer)) {
            statusBar()->showMessage(tr("Покупатель добавлен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось добавить покупателя"));
        }
    }
}

void MainWindow::editBuyer()
{
    QModelIndexList indexes = view->selectionModel()->selectedRows();
    if (indexes.isEmpty()) {
        QMessageBox::warning(this, tr("Предупреждение"),
            tr("Пожалуйста, выберите покупателя для редактирования"));
        return;
    }

    int row = indexes.first().row();
    QString lastName, firstName, middleName;
    QString passportNumber, phoneNumber, email;
    QString propertyType, propertyAddress;
    double propertyArea, propertyPrice;
    QDate purchaseDate;
    QString paymentStatus, contractNumber;

    Buyer buyer = controller->getBuyer(row);
    lastName = buyer.lastName;
    firstName = buyer.firstName;
    middleName = buyer.middleName;
    passportNumber = buyer.passportNumber;
    phoneNumber = buyer.phoneNumber;
    email = buyer.email;
    propertyType = buyer.propertyType;
    propertyAddress = buyer.propertyAddress;
    propertyArea = buyer.propertyArea.toDouble();
    propertyPrice = buyer.propertyPrice.toDouble();
    purchaseDate = QDate::fromString(buyer.purchaseDate, "yyyy-MM-dd");
    paymentStatus = buyer.paymentStatus;
    contractNumber = buyer.contractNumber;

    BuyerDialog dialog(this);
    dialog.setBuyerData(lastName, firstName, middleName,
                      passportNumber, phoneNumber, email,
                      propertyType, propertyAddress, propertyArea,
                      propertyPrice, purchaseDate, paymentStatus,
                      contractNumber);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.getBuyerData(lastName, firstName, middleName,
                          passportNumber, phoneNumber, email,
                          propertyType, propertyAddress, propertyArea,
                          propertyPrice, purchaseDate, paymentStatus,
                          contractNumber);

        Buyer updatedBuyer;
        updatedBuyer.lastName = lastName;
        updatedBuyer.firstName = firstName;
        updatedBuyer.middleName = middleName;
        updatedBuyer.passportNumber = passportNumber;
        updatedBuyer.phoneNumber = phoneNumber;
        updatedBuyer.email = email;
        updatedBuyer.propertyType = propertyType;
        updatedBuyer.propertyAddress = propertyAddress;
        updatedBuyer.propertyArea = QString::number(propertyArea);
        updatedBuyer.propertyPrice = QString::number(propertyPrice);
        updatedBuyer.purchaseDate = purchaseDate.toString("yyyy-MM-dd");
        updatedBuyer.paymentStatus = paymentStatus;
        updatedBuyer.contractNumber = contractNumber;

        if (controller->updateBuyer(row, updatedBuyer)) {
            statusBar()->showMessage(tr("Покупатель обновлен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось обновить покупателя"));
        }
    }
}

void MainWindow::deleteBuyer()
{
    QModelIndexList indexes = view->selectionModel()->selectedRows();
    if (indexes.isEmpty()) {
        QMessageBox::warning(this, tr("Предупреждение"),
            tr("Пожалуйста, выберите покупателя для удаления"));
        return;
    }

    int row = indexes.first().row();
    if (QMessageBox::question(this, tr("Подтверждение удаления"),
        tr("Вы уверены, что хотите удалить этого покупателя?"),
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (controller->deleteBuyer(row)) {
            statusBar()->showMessage(tr("Покупатель удален"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось удалить покупателя"));
        }
    }
}

void MainWindow::about()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::changeLanguage()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString language = action->text().toLower();
        if (translator.load(QString(":/translations/buyerdatabase_%1").arg(language))) {
            qApp->installTranslator(&translator);
            ui->retranslateUi(this);
        }
    }
}

void MainWindow::searchBuyers()
{
    bool ok;
    QString searchText = QInputDialog::getText(this, tr("Поиск"),
        tr("Введите текст поиска:"), QLineEdit::Normal,
        QString(), &ok);

    if (ok && !searchText.isEmpty()) {
        int column = view->currentIndex().column();
        if (column == -1) {
            QStringList columns;
            for (int i = 0; i < model->columnCount(); ++i) {
                columns << model->headerData(i, Qt::Horizontal).toString();
            }
            column = QInputDialog::getItem(this, tr("Выберите столбец"),
                tr("Поиск в столбце:"), columns, 0, false, &ok).toInt();
        }

        if (ok) {
            QVector<Buyer> results = controller->searchBuyers(searchText, column);
            if (!results.isEmpty()) {
                // Найдите строку первого результата в модели
                for (int i = 0; i < model->rowCount(); ++i) {
                    if (model->getBuyer(i).passportNumber == results.first().passportNumber) {
                        view->selectRow(i);
                        break;
                    }
                }
                statusBar()->showMessage(tr("Найдено %1 совпадений").arg(results.size()), 2000);
            } else {
                statusBar()->showMessage(tr("Совпадений не найдено"), 2000);
            }
        }
    }
}

void MainWindow::printView()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        view->render(&painter);
    }
}

void MainWindow::showChart()
{
    QChart *chart = new QChart();
    chart->setTitle(tr("Распределение типов недвижимости"));

    QBarSet *set = new QBarSet(tr("Количество недвижимости"));
    QStringList categories;

    // Получите данные из модели
    QMap<QString, int> propertyTypes;
    for (int i = 0; i < model->rowCount(); ++i) {
        QString type = model->data(model->index(i, 6)).toString();
        propertyTypes[type]++;
    }

    // Добавьте данные на график
    for (auto it = propertyTypes.begin(); it != propertyTypes.end(); ++it) {
        *set << it.value();
        categories << it.key();
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, set->at(set->count() - 1) * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Распределение типов недвижимости"));
    dialog->resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);

    dialog->exec();
}

void MainWindow::updateWindowTitle()
{
    QString title = tr("База данных покупателей недвижимости");
    if (!currentFile.isEmpty()) {
        title += " - " + QFileInfo(currentFile).fileName();
    }
    setWindowTitle(title);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
} 