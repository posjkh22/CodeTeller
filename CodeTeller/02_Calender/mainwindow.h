#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QFile>
#include <QTextEdit>
#include <QProcess>
#include <iostream>


class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLabel;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();


private slots:
    void localeChanged(int index);
    void firstDayChanged(int index);
    void selectionModeChanged(int index);
    void horizontalHeaderChanged(int index);
    void verticalHeaderChanged(int index);
    void selectedDateChanged();
    void minimumDateChanged(const QDate &date);
    void maximumDateChanged(const QDate &date);
    void weekdayFormatChanged();
    void weekendFormatChanged();
    void reformatHeaders();
    void reformatCalendarPage();
    void handleCodeViewer();
    void handleAnalysis();
    void handleGraphExpand();

    void threadTypeChanged(int index);
    void targetTypeChanged(int index);
    void AnalysisTypeChanged(int index);


private:
    void createCodeAnalysisGroupBox();
    void createCodeViewerBox();
    void createTempCalender();

    void createGeneralOptionsGroupBox();
    void createTextFormatsGroupBox();
    void createAnalysisResultGroupBox();
    void createAnalysisPathGroupBox();


    QComboBox *createColorComboBox();

    QString sourceFileName;
    QString parsedBitcodeSourceFileName;
    QStringList parsedBitcodeSourceFileNameList;

    QFile sourceFile;
    QTextEdit *sourceCodeViewer;


    QGroupBox* UpperGroupBox;
    QGridLayout *UpperLayout;

    QGroupBox* LowerGroupBox;
    QGridLayout *LowerLayout;



    QGroupBox* codeAnalysisBox;
    QGridLayout *codeAnalysisLayout;
    QGroupBox* codeAnalysisRunBox;
    QGridLayout *codeAnalysisRunLayout;
    QGroupBox* codeAnalysisSettingBox;
    QGridLayout *codeAnalysisSettingLayout;
    QGroupBox* codeAnalysisGraphBox;
    QGridLayout *codeAnalysisGraphLayout;



    QString InnerProgramCodeAnalysis;
    QString InnerProgramDrawCallGraph;
    QString AnalysisResults;

    QGroupBox *tempCalenderBox;
    QGridLayout *tempCalenderLayout;

    QGroupBox *codeViewerBox;
    QGridLayout *codeViewerLayout;

    QGroupBox *AnalysisResultBox;
    QGridLayout *AnalysisResultLayout;
    QTextEdit *AnalysisResultViewer;
    QString AnalysisResultFileName;
    QFile AnalysisResultFile;

    QGroupBox *codeAnalysisResultBox3;
    QGridLayout *AnalysisLayout3;

    QGroupBox *codeAnalysisPathBox;
    QGridLayout *AnalysisPathLayout;
    QTextEdit *AnalysisPathViewer;
    QString AnalysisPathFileName;
    QFile AnalysisPathFile;

    QGroupBox *codeAnalysisGlobalVariablesBox;
    QGridLayout *AnalysisGlobalVariablesLayout;
    QTextEdit *AnalysisGlobalVariablesViewer;
    QString AnalysisGlobalVariablesFileName;
    QFile AnalysisGlobalVariablesFile;


    QLabel* analysisGraphLabel;
    QPixmap analysisGraph;
    QString analysisGraphName;

    QLabel* sourceCodeNameViewLabel;

    QLabel* threadTypeLabel;
    QLabel* targetTypeLabel;



    QProcess *main_process;
    QProcess *CallgraphDraw;
    QProcess *bitcodeGen;


    QString threadOption;
    QString targetOption;
    QString AnalysisOption;


    QGridLayout *previewLayout;




    QCalendarWidget *calendar;

    QGroupBox *generalOptionsGroupBox;
    QLabel *localeLabel;
    QLabel *firstDayLabel;
    QLabel *selectionModeLabel;
    QLabel *horizontalHeaderLabel;
    QLabel *verticalHeaderLabel;

    QLabel *threadLabel;
    QLabel *targetLabel;
    QLabel *AnalysisLabel;


    QComboBox *threadCombo;
    QComboBox *targetCombo;
    QComboBox *AnalysisCombo;


    QComboBox *localeCombo;
    QComboBox *firstDayCombo;
    QComboBox *selectionModeCombo;
    QCheckBox *gridCheckBox;
    QCheckBox *navigationCheckBox;
    QComboBox *horizontalHeaderCombo;
    QComboBox *verticalHeaderCombo;

    QLabel *currentDateLabel;
    QLabel *minimumDateLabel;
    QLabel *maximumDateLabel;
    QDateEdit *currentDateEdit;
    QDateEdit *minimumDateEdit;
    QDateEdit *maximumDateEdit;

    QGroupBox *textFormatsGroupBox;
    QLabel *weekdayColorLabel;
    QLabel *weekendColorLabel;
    QLabel *headerTextFormatLabel;
    QComboBox *weekdayColorCombo;
    QComboBox *weekendColorCombo;
    QComboBox *headerTextFormatCombo;

    QCheckBox *firstFridayCheckBox;
    QCheckBox *mayFirstCheckBox;
};

#endif
