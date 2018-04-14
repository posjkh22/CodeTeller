#include <QtWidgets>
#include <QTextEdit>
#include <QDir>
#include <QPixmap>
#include "mainwindow.h"

Window::~Window()
{
    system("rm -f *.bc AnalysisResults graph.png graph.dot");
}


Window::Window()
{
    createCodeViewerBox();
    createCodeAnalysisGroupBox();
    createAnalysisResultGroupBox();
    createAnalysisPathGroupBox();

    /*
    createTempCalender();
    createGeneralOptionsGroupBox();
    createTextFormatsGroupBox();
    */

    QGridLayout *layout = new QGridLayout;
    QGridLayout *UpperLayout = new QGridLayout;
    QGridLayout *LowerLayout = new QGridLayout;

    UpperGroupBox = new QGroupBox;
    LowerGroupBox = new QGroupBox;

    UpperGroupBox->setLayout(UpperLayout);
    LowerGroupBox->setLayout(LowerLayout);

    UpperLayout->addWidget(codeViewerBox, 0, 0);
    UpperLayout->addWidget(codeAnalysisBox, 0, 1);
    UpperLayout->addWidget(codeAnalysisResultBox3, 0, 2);

    LowerLayout->addWidget(codeAnalysisPathBox, 0, 0);


    layout->addWidget(UpperGroupBox, 0, 0);
    layout->addWidget(LowerGroupBox, 1, 0);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout);
    setWindowTitle(tr("Code Analysis Widget"));
}



void Window::threadTypeChanged(int index)
{
    if(index == 0)
    {
        threadOption = QString("--thread=Multithread");
    }
    else if(index == 1)
    {
        threadOption = QString("--thread=Unithread");
    }
    else
    {
        threadOption = QString("--thread");
    }
}

void Window::targetTypeChanged(int index)
{
    if(index == 0)
    {
        targetOption = QString("--target=Linux");
    }
    else if(index == 1)
    {
        targetOption = QString("--target=OSEC");
    }
    else if(index == 2)
    {
        targetOption = QString("--target=FreeRTOS");
    }
    else if(index == 3)
    {
        targetOption = QString("--target=MicroC/OS-II");
    }
    else
    {
        targetOption = QString("Target Type: Error");
    }

}

void Window::AnalysisTypeChanged(int index)
{
    if(index == 0)
    {
        AnalysisOption = QString("--analysis=MemoryAllocationC");
    }
    else if(index == 1)
    {
        AnalysisOption = QString("--analysis=FilePointerAnalysisC");
    }
    else if(index == 2)
    {
        AnalysisOption = QString("--analysis=Deadlock");
    }
    else if(index == 3)
    {
        AnalysisOption = QString("--analysis=SharedVariables");
    }
    else if(index == 4)
    {
        AnalysisOption = QString("--analysis=SharedFunctions");
    }
    else
    {
        AnalysisOption = QString("--anlysis");
    }
}




void Window::localeChanged(int index)
{
    const QLocale newLocale(localeCombo->itemData(index).toLocale());
    calendar->setLocale(newLocale);
    int newLocaleFirstDayIndex = firstDayCombo->findData(newLocale.firstDayOfWeek());
    firstDayCombo->setCurrentIndex(newLocaleFirstDayIndex);
}

void Window::firstDayChanged(int index)
{
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(
                                firstDayCombo->itemData(index).toInt()));
}

void Window::selectionModeChanged(int index)
{
    calendar->setSelectionMode(QCalendarWidget::SelectionMode(
                               selectionModeCombo->itemData(index).toInt()));
}

void Window::horizontalHeaderChanged(int index)
{
    calendar->setHorizontalHeaderFormat(QCalendarWidget::HorizontalHeaderFormat(
        horizontalHeaderCombo->itemData(index).toInt()));
}

void Window::verticalHeaderChanged(int index)
{
    calendar->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(
        verticalHeaderCombo->itemData(index).toInt()));
}

void Window::selectedDateChanged()
{
    currentDateEdit->setDate(calendar->selectedDate());
}

void Window::minimumDateChanged(const QDate &date)
{
    calendar->setMinimumDate(date);
    maximumDateEdit->setDate(calendar->maximumDate());
}

void Window::maximumDateChanged(const QDate &date)
{
    calendar->setMaximumDate(date);
    minimumDateEdit->setDate(calendar->minimumDate());
}

void Window::weekdayFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
        weekdayColorCombo->itemData(weekdayColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Monday, format);
    calendar->setWeekdayTextFormat(Qt::Tuesday, format);
    calendar->setWeekdayTextFormat(Qt::Wednesday, format);
    calendar->setWeekdayTextFormat(Qt::Thursday, format);
    calendar->setWeekdayTextFormat(Qt::Friday, format);
}

void Window::weekendFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
        weekendColorCombo->itemData(weekendColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Saturday, format);
    calendar->setWeekdayTextFormat(Qt::Sunday, format);
}

void Window::reformatHeaders()
{
    QString text = headerTextFormatCombo->currentText();
    QTextCharFormat format;

    if (text == tr("Bold")) {
        format.setFontWeight(QFont::Bold);
    } else if (text == tr("Italic")) {
        format.setFontItalic(true);
    } else if (text == tr("Green")) {
        format.setForeground(Qt::green);
    }
    calendar->setHeaderTextFormat(format);
}

void Window::reformatCalendarPage()
{
    QTextCharFormat mayFirstFormat;
    const QDate mayFirst(calendar->yearShown(), 5, 1);

    QTextCharFormat firstFridayFormat;
    QDate firstFriday(calendar->yearShown(), calendar->monthShown(), 1);
    while (firstFriday.dayOfWeek() != Qt::Friday)
        firstFriday = firstFriday.addDays(1);

    if (firstFridayCheckBox->isChecked()) {
        firstFridayFormat.setForeground(Qt::blue);
    } else { // Revert to regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(firstFriday.dayOfWeek()));
        firstFridayFormat.setForeground(calendar->weekdayTextFormat(dayOfWeek).foreground());
    }

    calendar->setDateTextFormat(firstFriday, firstFridayFormat);

    // When it is checked, "May First in Red" always takes precedence over "First Friday in Blue".
    if (mayFirstCheckBox->isChecked()) {
        mayFirstFormat.setForeground(Qt::red);
    } else if (!firstFridayCheckBox->isChecked() || firstFriday != mayFirst) {
        // We can now be certain we won't be resetting "May First in Red" when we restore
        // may 1st's regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(mayFirst.dayOfWeek()));
        calendar->setDateTextFormat(mayFirst, calendar->weekdayTextFormat(dayOfWeek));
    }

    calendar->setDateTextFormat(mayFirst, mayFirstFormat);
}

void Window::createAnalysisPathGroupBox()
{



    // AnalysisPathViewer //
    codeAnalysisPathBox = new QGroupBox(tr("Analysis Path"));
    AnalysisPathLayout = new QGridLayout;
    AnalysisPathViewer = new QTextEdit();





    // AnalysisPathViewer //
    AnalysisPathViewer = new QTextEdit();
    const QSize ANALYSIS_PATH_FIXED_SIZE = QSize(1150, 100);
    AnalysisPathViewer->setFixedSize(ANALYSIS_PATH_FIXED_SIZE);
    const QSize ANALYSIS_PATH_MAX_SIZE = QSize(1150, 100);
    AnalysisPathViewer->setMaximumSize(ANALYSIS_PATH_MAX_SIZE);

    AnalysisPathViewer->clear();
    AnalysisPathLayout->addWidget(AnalysisPathViewer, 0, 0, Qt::AlignCenter);
    codeAnalysisPathBox->setLayout(AnalysisPathLayout);

    // View Analysis Results //
    AnalysisPathFileName = QString("AnalysisPaths");

    if(!AnalysisPathFileName.isNull())
    {
        system("echo 'ERROR:Improper Analysis Setting' >> AnalysisPaths" );
        qDebug() << "selected file path: " << AnalysisPathFileName.toUtf8();
    }

    // View source Code //
    AnalysisPathViewer->clear();
    AnalysisPathFile.setFileName(AnalysisPathFileName);

    if(!AnalysisPathFile.exists())
    {
        qDebug() << "No exists file: " << AnalysisPathFileName;
    }
    else
    {
        qDebug() << AnalysisResultFileName << " open";
    }







    AnalysisPathLayout->addWidget(AnalysisPathViewer, 0, 0, Qt::AlignCenter);
    codeAnalysisPathBox->setLayout(AnalysisPathLayout);


}

void Window::createAnalysisResultGroupBox()
{

    codeAnalysisResultBox3 = new QGroupBox();
    AnalysisLayout3 = new QGridLayout;


    // 1.
    AnalysisResultBox = new QGroupBox(tr("Analysis Result"));
    AnalysisResultLayout = new QGridLayout;
    //AnalysisResultFile = new QFile();

    AnalysisResultViewer = new QTextEdit();
    AnalysisResultViewer->setFontPointSize(8.5);

    QFont AnalysisResultViewerFont = AnalysisResultViewer->currentFont();
    QFontMetrics metrics(AnalysisResultViewerFont);
    AnalysisResultViewer->setFontPointSize(8.0);
    AnalysisResultViewer->setTabStopWidth(4 * metrics.width(' '));

    AnalysisResultViewer->clear();

    const QSize ANALYSIS_RESULTS_SIZE = QSize(300, 350);
    AnalysisResultViewer->setFixedSize(ANALYSIS_RESULTS_SIZE);

    AnalysisResultLayout->addWidget(AnalysisResultViewer, 2, 0, Qt::AlignCenter);
    AnalysisResultBox->setLayout(AnalysisResultLayout);

    // View Analysis Results //
    AnalysisResultFileName = QString("AnalysisResults");

    if(!AnalysisResultFileName.isNull())
    {
        system("echo 'ERROR:Improper Analysis Setting' >> AnalysisResults" );
        qDebug() << "selected file path: " << AnalysisResultFileName.toUtf8();
    }

    // View source Code //
    AnalysisResultViewer->clear();
    AnalysisResultFile.setFileName(AnalysisResultFileName);

    if(!AnalysisResultFile.exists())
    {
        qDebug() << "No exists file: " << AnalysisResultFileName;
    }
    else
    {
        qDebug() << AnalysisResultFileName << " open";
    }



    // 2.

    // AnalysisVariablesViewer //
    codeAnalysisGlobalVariablesBox = new QGroupBox(tr("Global Variables"));
    AnalysisGlobalVariablesLayout = new QGridLayout;
    AnalysisGlobalVariablesViewer = new QTextEdit();

    // AnalysisGlobalVariablesViewer //
    AnalysisGlobalVariablesViewer = new QTextEdit();
    const QSize ANALYSIS_GVAR_SIZE = QSize(300, 200);
    AnalysisGlobalVariablesViewer->setFixedSize(ANALYSIS_GVAR_SIZE);


    AnalysisGlobalVariablesViewer->clear();
    AnalysisGlobalVariablesLayout->addWidget(AnalysisGlobalVariablesViewer, 0, 0, Qt::AlignCenter);
    codeAnalysisGlobalVariablesBox->setLayout(AnalysisGlobalVariablesLayout);

    // View Analysis GlobalVariables //
    AnalysisGlobalVariablesFileName = QString("AnalysisGlobalVariables");

    if(!AnalysisGlobalVariablesFileName.isNull())
    {
        system("echo 'ERROR:Improper Analysis Setting' >> AnalysisPaths" );
        qDebug() << "selected file path: " << AnalysisGlobalVariablesFileName.toUtf8();
    }

    // View source Code //
    AnalysisGlobalVariablesViewer->clear();
    AnalysisGlobalVariablesFile.setFileName(AnalysisGlobalVariablesFileName);

    if(!AnalysisGlobalVariablesFile.exists())
    {
        qDebug() << "No exists file: " << AnalysisGlobalVariablesFileName;
    }
    else
    {
        qDebug() << AnalysisResultFileName << " open";
    }


    // Summary //
    AnalysisLayout3->addWidget(AnalysisResultBox, 0, 0, Qt::AlignCenter);
    AnalysisLayout3->addWidget(codeAnalysisGlobalVariablesBox, 1, 0, Qt::AlignCenter);
    codeAnalysisResultBox3->setLayout(AnalysisLayout3);

}


void Window::createTempCalender()
{
    tempCalenderBox = new QGroupBox(tr("tempCalender"));
    tempCalenderLayout = new QGridLayout;


    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);

    connect(calendar, SIGNAL(currentPageChanged(int,int)),
            this, SLOT(reformatCalendarPage()));

    tempCalenderLayout->addWidget(calendar, 0, 0, Qt::AlignCenter);
    tempCalenderBox->setLayout(tempCalenderLayout);

}

void Window::handleGraphExpand()
{
    system("display graph.png");
}



void Window::handleAnalysis()
{
    // Make Analysis //
    InnerProgramCodeAnalysis = QString("/code_analysis");
    InnerProgramDrawCallGraph = QString("/draw_callgraph");

    AnalysisResults = QString("AnalysisResults");

    QString file1 = QDir::currentPath() + InnerProgramCodeAnalysis+ " " + QDir::currentPath() + "/" +parsedBitcodeSourceFileName + " "
            + threadOption + " " + targetOption + " " + AnalysisOption ;
    qDebug() << "Analysis: " + file1;
    main_process->start(file1);
    main_process->waitForFinished();



    // Make CallGraph //
    QString file2 = QDir::currentPath() + InnerProgramDrawCallGraph+ " " + QDir::currentPath() + "/" +parsedBitcodeSourceFileName;
    qDebug() << "DrawCallgraph: " +file2;
    CallgraphDraw->start(file2);
    CallgraphDraw->waitForFinished();

    // View Analysis Results //
    AnalysisResultViewer->clear();

    QString line;
    if(AnalysisResultFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&AnalysisResultFile);
        while(!stream.atEnd())
        {
            line = stream.readLine();
            AnalysisResultViewer->setText(AnalysisResultViewer->toPlainText()+line+"\n");
            qDebug() << "line: " << line;
        }
    }
    AnalysisResultFile.close();

    // Reset AnalysisResultFile //
    system("rm -f AnalysisResults");



    // View Analysis Paths //
    AnalysisPathViewer->clear();

    QString path_line;
    if(AnalysisPathFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&AnalysisPathFile);
        while(!stream.atEnd())
        {
            path_line = stream.readLine();
            AnalysisPathViewer->setText(AnalysisPathViewer->toPlainText()+path_line+"\n");
            qDebug() << "line: " << path_line;
        }
    }
    AnalysisPathFile.close();

    // Reset AnalysisPathFile
    system("rm -f AnalysisPaths");


    // View Analysis Paths //
    AnalysisGlobalVariablesViewer->clear();

    QString global_variable_line;
    if(AnalysisGlobalVariablesFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&AnalysisGlobalVariablesFile);
        while(!stream.atEnd())
        {
            global_variable_line = stream.readLine();
            AnalysisGlobalVariablesViewer->setText(AnalysisGlobalVariablesViewer->toPlainText()+global_variable_line+"\n");
            qDebug() << "line: " << global_variable_line;
        }
    }
    AnalysisGlobalVariablesFile.close();

    // Reset AnalysisPathFile
    system("rm -f AnalysisGlobalVariables");



    // View Analysis Graph //
    analysisGraphName = QString(QDir::currentPath() + "/graph.png");
    qDebug() << "Image Load: " +analysisGraphName;
    analysisGraph.load(analysisGraphName);
    analysisGraphLabel->setPixmap(analysisGraph.scaled(analysisGraphLabel->width(),
                                                       analysisGraphLabel->height(), Qt::IgnoreAspectRatio));

}


void Window::createCodeAnalysisGroupBox()
{

    threadOption = QString("--thread=Multithread");
    targetOption = QString("--target=Linux");
    AnalysisOption = QString("--analysis=MemoryAllocationC");

    // Layout //
    codeAnalysisBox = new QGroupBox(tr("CodeAnalysis"));
    codeAnalysisLayout = new QGridLayout;

    codeAnalysisSettingBox = new QGroupBox(tr("Setting"));

    const QSize SETTINGBOX_SIZE = QSize(500, 120);
    codeAnalysisSettingBox->setFixedSize(SETTINGBOX_SIZE);

    codeAnalysisSettingLayout = new QGridLayout;


    codeAnalysisGraphBox = new QGroupBox(tr("Graph"));
    codeAnalysisGraphLayout = new QGridLayout;

    main_process = new QProcess(this);

    CallgraphDraw = new QProcess(this);

    // Analysis start button //
    QPushButton *code_analysis_button = new QPushButton();
    code_analysis_button = new QPushButton("Code Analysis");
    //code_analysis_button->setStyleSheet("border: 1px solid black; background: white");
    const QSize BUTTON_SIZE = QSize(500, 30);
    code_analysis_button->setFixedSize(BUTTON_SIZE);
    connect(code_analysis_button, SIGNAL (released()), this, SLOT (handleAnalysis()));
    codeAnalysisLayout->addWidget(code_analysis_button, 0, 0, Qt::AlignCenter);
    codeAnalysisBox->setLayout(codeAnalysisLayout);

    // graph expand button //
    QPushButton *graph_expand_button = new QPushButton();
    graph_expand_button = new QPushButton("Analysis Graph Expand");
    const QSize GRAPH_EXPAND_BUTTON_SIZE = QSize(500, 30);
    graph_expand_button->setFixedSize(GRAPH_EXPAND_BUTTON_SIZE);
    connect(graph_expand_button, SIGNAL (released()), this, SLOT (handleGraphExpand()));
    codeAnalysisLayout->addWidget(graph_expand_button, 3, 0, Qt::AlignCenter);
    codeAnalysisBox->setLayout(codeAnalysisLayout);


    // threadCombo box //
    threadCombo = new QComboBox;

    threadLabel = new QLabel(tr("&Thread Type"));
    threadLabel->setBuddy(threadCombo);

    threadCombo->addItem(tr("Multi-thread"), Qt::Sunday);
    threadCombo->addItem(tr("Uni-thread"), Qt::Monday);


    // targetCombo box //
    targetCombo = new QComboBox;

    targetLabel = new QLabel(tr("&OS Type"));
    targetLabel->setBuddy(targetCombo);

    targetCombo->addItem(tr("Linux"), Qt::Sunday);
    targetCombo->addItem(tr("OSEC"), Qt::Monday);
    targetCombo->addItem(tr("FreeRTOS"), Qt::Monday);
    targetCombo->addItem(tr("MicroC/OS-II"), Qt::Monday);


    // AnalysisCombo box //
    AnalysisCombo = new QComboBox;

    AnalysisLabel = new QLabel(tr("&Analysis Type"));
    AnalysisLabel->setBuddy(AnalysisCombo);

    AnalysisCombo->addItem(tr("malloc/free"), Qt::Sunday);
    AnalysisCombo->addItem(tr("fopen/fclose"), Qt::Sunday);
    AnalysisCombo->addItem(tr("Deadlock"), Qt::Monday);
    AnalysisCombo->addItem(tr("Semaphore Integrity"), Qt::Monday);
    AnalysisCombo->addItem(tr("Shared Variables"), Qt::Monday);
    AnalysisCombo->addItem(tr("Shared Functions"), Qt::Monday);

    connect(threadCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(threadTypeChanged(int)));
    connect(targetCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(targetTypeChanged(int)));
    connect(AnalysisCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(AnalysisTypeChanged(int)));


    codeAnalysisSettingLayout->addWidget(threadLabel, 0, 0);
    codeAnalysisSettingLayout->addWidget(threadCombo, 0, 1);
    codeAnalysisSettingLayout->addWidget(targetLabel, 1, 0);
    codeAnalysisSettingLayout->addWidget(targetCombo, 1, 1);
    codeAnalysisSettingLayout->addWidget(AnalysisLabel, 2, 0);
    codeAnalysisSettingLayout->addWidget(AnalysisCombo, 2, 1);


    codeAnalysisSettingBox->setLayout(codeAnalysisSettingLayout);



    // Show the image //

    analysisGraphName = QString(QDir::currentPath() + "/default_image.png");
    analysisGraph.load(analysisGraphName);

    qDebug() << "image path: " << analysisGraphName;

    analysisGraphLabel = new QLabel();

    const QSize GRAPH_SIZE = QSize(475, 375);
    analysisGraphLabel->setFixedSize(GRAPH_SIZE);

    analysisGraphLabel->setPixmap(analysisGraph.scaled(analysisGraphLabel->width(),
                                                       analysisGraphLabel->height(), Qt::IgnoreAspectRatio));

    codeAnalysisGraphLayout->addWidget(analysisGraphLabel, 0, 0, Qt::AlignCenter);


    codeAnalysisGraphBox->setLayout(codeAnalysisGraphLayout);

    codeAnalysisLayout->addWidget(codeAnalysisSettingBox, 1, 0, Qt::AlignCenter);
    codeAnalysisLayout->addWidget(codeAnalysisGraphBox, 2, 0, Qt::AlignCenter);

    codeAnalysisBox->setLayout(codeAnalysisLayout);



}


void Window::handleCodeViewer()
{
    // Pop FileDiaglog //
    sourceFileName = QFileDialog::getOpenFileName(
                this,
                "Open Source code",
                QDir::currentPath(),
                "All files (*.*) ;; cpp files (*.cpp);; c files (*.c);; header files (*.h)");

    if(!sourceFileName.isNull())
    {
        qDebug() << "selected file path: " << sourceFileName.toUtf8();
    }

    QStringList sourceFileNameList = sourceFileName.split("/");
    int SOURCEFILENAMELISTSIZE = sourceFileNameList.size();
    QString parsedsourceFileName = sourceFileNameList.at(SOURCEFILENAMELISTSIZE-1);

    // Label source Code //
    sourceCodeNameViewLabel->setText("Source code: " + parsedsourceFileName.toUtf8());

    // Generate bitcode //

    parsedBitcodeSourceFileNameList = parsedsourceFileName.split(".");
    parsedBitcodeSourceFileName = parsedBitcodeSourceFileNameList.at(0) + ".bc";
    QString CLANG_PATH("/home/posjkh22/LLVM/llvm-ubuntu-linux-5.0.0/bin/");

    QString file = CLANG_PATH + "clang -g -S -emit-llvm -o " + QDir::currentPath() + "/" + parsedBitcodeSourceFileName
            + " " + QDir::currentPath() + "/" +  parsedsourceFileName;
    qDebug() << "running: " << file;

    bitcodeGen = new QProcess(this);
    bitcodeGen->start(file);
    bitcodeGen->waitForFinished();




    // View source Code //
    sourceCodeViewer->clear();
    sourceFile.setFileName(sourceFileName);

    if(!sourceFile.exists())
    {
        qDebug() << "No exists file: " << sourceFileName;
    }
    else
    {
        qDebug() << sourceFileName << " open";
    }

    QString line;
    if(sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&sourceFile);

        int lineNumber = 1;
        QString lineNumberQString;
        while(!stream.atEnd())
        {
            line = stream.readLine();
            sourceCodeViewer->setText(sourceCodeViewer->toPlainText()+ lineNumberQString.setNum(lineNumber++) +": " + line+"\n");
            qDebug() << "line: " << line;
        }
    }
    sourceFile.close();

    // Reset: Analysis path //
    AnalysisPathViewer->clear();

    // Reset: Analysis Graph & Analysis Results //
    AnalysisResultViewer->clear();
    analysisGraphName = QString(QDir::currentPath() + "/default_image.png");
    analysisGraph.load(analysisGraphName);
    analysisGraphLabel->setPixmap(analysisGraph.scaled(analysisGraphLabel->width(),
                                                       analysisGraphLabel->height(), Qt::IgnoreAspectRatio));

    system("rm -f AnalysisResults graph.png graph.dot");
}


void Window::createCodeViewerBox()
{

    codeViewerBox = new QGroupBox(tr("Source code"));

    // Button: Source code selector //
    codeViewerLayout = new QGridLayout;

    QPushButton *code_sel_button = new QPushButton();
    code_sel_button = new QPushButton("Code Selector");
    //code_sel_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

    //code_sel_button->setStyleSheet("border: 1px solid black; background: white");
    const QSize BUTTON_SIZE = QSize(300, 30);
    code_sel_button->setFixedSize(BUTTON_SIZE);




    connect(code_sel_button, SIGNAL (released()), this, SLOT (handleCodeViewer()));
    codeViewerLayout->addWidget(code_sel_button, 0, 0, Qt::AlignCenter);
    codeViewerBox->setLayout(codeViewerLayout);


    // Label: SourceCodeName View //
    sourceCodeNameViewLabel = new QLabel(QString("Source code: "));
    codeViewerLayout->addWidget(sourceCodeNameViewLabel, 1, 0, Qt::AlignLeft);
    codeViewerBox->setLayout(codeViewerLayout);


    // TextEdit: Source code viewer //
    sourceCodeViewer = new QTextEdit();
    QFont sourceCodeViewerFont = sourceCodeViewer->currentFont();
    QFontMetrics metrics(sourceCodeViewerFont);
    sourceCodeViewer->setFontPointSize(8.5);
    sourceCodeViewer->setTabStopWidth(4 * metrics.width(' '));

    const QSize CODE_VIEWER_SIZE = QSize(300, 500);
    sourceCodeViewer->setFixedSize(CODE_VIEWER_SIZE);

    sourceCodeViewer->clear();

    codeViewerLayout->addWidget(sourceCodeViewer, 2, 0, Qt::AlignCenter);
    codeViewerBox->setLayout(codeViewerLayout);


    //sourceFile.close();


}



void Window::createGeneralOptionsGroupBox()
{
    generalOptionsGroupBox = new QGroupBox(tr("General Options"));


    localeCombo = new QComboBox;
    int curLocaleIndex = -1;
    int index = 0;
    for (int _lang = QLocale::C; _lang <= QLocale::LastLanguage; ++_lang) {
        QLocale::Language lang = static_cast<QLocale::Language>(_lang);
        QList<QLocale::Country> countries = QLocale::countriesForLanguage(lang);
        for (int i = 0; i < countries.count(); ++i) {
            QLocale::Country country = countries.at(i);
            QString label = QLocale::languageToString(lang);
            label += QLatin1Char('/');
            label += QLocale::countryToString(country);
            QLocale locale(lang, country);
            if (this->locale().language() == lang && this->locale().country() == country)
                curLocaleIndex = index;
            localeCombo->addItem(label, locale);
            ++index;
        }
    }
    if (curLocaleIndex != -1)
        localeCombo->setCurrentIndex(curLocaleIndex);
    localeLabel = new QLabel(tr("&Locale"));
    localeLabel->setBuddy(localeCombo);

    firstDayCombo = new QComboBox;
    firstDayCombo->addItem(tr("Sunday"), Qt::Sunday);
    firstDayCombo->addItem(tr("Monday"), Qt::Monday);
    firstDayCombo->addItem(tr("Tuesday"), Qt::Tuesday);
    firstDayCombo->addItem(tr("Wednesday"), Qt::Wednesday);
    firstDayCombo->addItem(tr("Thursday"), Qt::Thursday);
    firstDayCombo->addItem(tr("Friday"), Qt::Friday);
    firstDayCombo->addItem(tr("Saturday"), Qt::Saturday);

    firstDayLabel = new QLabel(tr("Wee&k starts on:"));
    firstDayLabel->setBuddy(firstDayCombo);

    selectionModeCombo = new QComboBox;
    selectionModeCombo->addItem(tr("Single selection"),
                                QCalendarWidget::SingleSelection);
    selectionModeCombo->addItem(tr("None"), QCalendarWidget::NoSelection);

    selectionModeLabel = new QLabel(tr("&Selection mode:"));
    selectionModeLabel->setBuddy(selectionModeCombo);

    gridCheckBox = new QCheckBox(tr("&Grid"));
    gridCheckBox->setChecked(calendar->isGridVisible());

    navigationCheckBox = new QCheckBox(tr("&Navigation bar"));
    navigationCheckBox->setChecked(true);

    horizontalHeaderCombo = new QComboBox;
    horizontalHeaderCombo->addItem(tr("Single letter day names"),
                                   QCalendarWidget::SingleLetterDayNames);
    horizontalHeaderCombo->addItem(tr("Short day names"),
                                   QCalendarWidget::ShortDayNames);
    horizontalHeaderCombo->addItem(tr("None"),
                                   QCalendarWidget::NoHorizontalHeader);
    horizontalHeaderCombo->setCurrentIndex(1);

    horizontalHeaderLabel = new QLabel(tr("&Horizontal header:"));
    horizontalHeaderLabel->setBuddy(horizontalHeaderCombo);

    verticalHeaderCombo = new QComboBox;
    verticalHeaderCombo->addItem(tr("ISO week numbers"),
                                 QCalendarWidget::ISOWeekNumbers);
    verticalHeaderCombo->addItem(tr("None"), QCalendarWidget::NoVerticalHeader);

    verticalHeaderLabel = new QLabel(tr("&Vertical header:"));
    verticalHeaderLabel->setBuddy(verticalHeaderCombo);

    connect(localeCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(localeChanged(int)));
    connect(firstDayCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(firstDayChanged(int)));
    connect(selectionModeCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(selectionModeChanged(int)));
    connect(gridCheckBox, SIGNAL(toggled(bool)),
            calendar, SLOT(setGridVisible(bool)));
    connect(navigationCheckBox, SIGNAL(toggled(bool)),
            calendar, SLOT(setNavigationBarVisible(bool)));
    connect(horizontalHeaderCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(horizontalHeaderChanged(int)));
    connect(verticalHeaderCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(verticalHeaderChanged(int)));


    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(gridCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(navigationCheckBox);


    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(localeLabel, 0, 0);
    outerLayout->addWidget(localeCombo, 0, 1);
    outerLayout->addWidget(firstDayLabel, 1, 0);
    outerLayout->addWidget(firstDayCombo, 1, 1);
    outerLayout->addWidget(selectionModeLabel, 2, 0);
    outerLayout->addWidget(selectionModeCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    outerLayout->addWidget(horizontalHeaderLabel, 4, 0);
    outerLayout->addWidget(horizontalHeaderCombo, 4, 1);
    outerLayout->addWidget(verticalHeaderLabel, 5, 0);
    outerLayout->addWidget(verticalHeaderCombo, 5, 1);

    generalOptionsGroupBox->setLayout(outerLayout);


    firstDayChanged(firstDayCombo->currentIndex());
    selectionModeChanged(selectionModeCombo->currentIndex());
    horizontalHeaderChanged(horizontalHeaderCombo->currentIndex());
    verticalHeaderChanged(verticalHeaderCombo->currentIndex());
}


void Window::createTextFormatsGroupBox()
{
    textFormatsGroupBox = new QGroupBox(tr("Text Formats"));

    weekdayColorCombo = createColorComboBox();
    weekdayColorCombo->setCurrentIndex(
            weekdayColorCombo->findText(tr("Black")));

    weekdayColorLabel = new QLabel(tr("&Weekday color:"));
    weekdayColorLabel->setBuddy(weekdayColorCombo);

    weekendColorCombo = createColorComboBox();
    weekendColorCombo->setCurrentIndex(
            weekendColorCombo->findText(tr("Red")));

    weekendColorLabel = new QLabel(tr("Week&end color:"));
    weekendColorLabel->setBuddy(weekendColorCombo);

    headerTextFormatCombo = new QComboBox;
    headerTextFormatCombo->addItem(tr("Bold"));
    headerTextFormatCombo->addItem(tr("Italic"));
    headerTextFormatCombo->addItem(tr("Plain"));

    headerTextFormatLabel = new QLabel(tr("&Header text:"));
    headerTextFormatLabel->setBuddy(headerTextFormatCombo);

    firstFridayCheckBox = new QCheckBox(tr("&First Friday in blue"));

    mayFirstCheckBox = new QCheckBox(tr("May &1 in red"));

    connect(weekdayColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(weekdayFormatChanged()));
    connect(weekdayColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(reformatCalendarPage()));
    connect(weekendColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(weekendFormatChanged()));
    connect(weekendColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(reformatCalendarPage()));
    connect(headerTextFormatCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(reformatHeaders()));
    connect(firstFridayCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(reformatCalendarPage()));
    connect(mayFirstCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(reformatCalendarPage()));

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(firstFridayCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(mayFirstCheckBox);

    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(weekdayColorLabel, 0, 0);
    outerLayout->addWidget(weekdayColorCombo, 0, 1);
    outerLayout->addWidget(weekendColorLabel, 1, 0);
    outerLayout->addWidget(weekendColorCombo, 1, 1);
    outerLayout->addWidget(headerTextFormatLabel, 2, 0);
    outerLayout->addWidget(headerTextFormatCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    textFormatsGroupBox->setLayout(outerLayout);

    weekdayFormatChanged();
    weekendFormatChanged();
    reformatHeaders();
    reformatCalendarPage();
}

QComboBox *Window::createColorComboBox()
{
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem(tr("Red"), QColor(Qt::red));
    comboBox->addItem(tr("Blue"), QColor(Qt::blue));
    comboBox->addItem(tr("Black"), QColor(Qt::black));
    comboBox->addItem(tr("Magenta"), QColor(Qt::magenta));
    return comboBox;
}
