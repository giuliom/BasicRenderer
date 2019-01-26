/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "QRenderingWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionRender;
    QAction *actionOpen;
    QAction *actionSave;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QRenderingWidget *renderWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *renderButton;
    QComboBox *rendererComboBox;
    QComboBox *shadingComboBox;
    QLabel *wlabel;
    QLineEdit *widthEdit;
    QLabel *hlabel;
    QLineEdit *heightEdit;
    QPushButton *saveButton;
    QSpacerItem *horizontalSpacer;
    QLabel *fpsLabel;
    QMenuBar *menuBar;
    QMenu *menuBoh;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(640, 480);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(5);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionRender = new QAction(MainWindow);
        actionRender->setObjectName(QStringLiteral("actionRender"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMinimumSize(QSize(330, 240));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        renderWidget = new QRenderingWidget(centralWidget);
        renderWidget->setObjectName(QStringLiteral("renderWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(renderWidget->sizePolicy().hasHeightForWidth());
        renderWidget->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(renderWidget, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        renderButton = new QPushButton(centralWidget);
        renderButton->setObjectName(QStringLiteral("renderButton"));
        renderButton->setMinimumSize(QSize(0, 25));
        renderButton->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(renderButton);

        rendererComboBox = new QComboBox(centralWidget);
        rendererComboBox->setObjectName(QStringLiteral("rendererComboBox"));
        rendererComboBox->setMinimumSize(QSize(0, 24));
        rendererComboBox->setMaximumSize(QSize(16777215, 24));

        horizontalLayout->addWidget(rendererComboBox);

        shadingComboBox = new QComboBox(centralWidget);
        shadingComboBox->setObjectName(QStringLiteral("shadingComboBox"));
        shadingComboBox->setMinimumSize(QSize(0, 24));
        shadingComboBox->setMaximumSize(QSize(16777215, 24));

        horizontalLayout->addWidget(shadingComboBox);

        wlabel = new QLabel(centralWidget);
        wlabel->setObjectName(QStringLiteral("wlabel"));
        wlabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(wlabel);

        widthEdit = new QLineEdit(centralWidget);
        widthEdit->setObjectName(QStringLiteral("widthEdit"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widthEdit->sizePolicy().hasHeightForWidth());
        widthEdit->setSizePolicy(sizePolicy3);
        widthEdit->setMinimumSize(QSize(40, 24));
        widthEdit->setMaximumSize(QSize(80, 24));
        widthEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(widthEdit);

        hlabel = new QLabel(centralWidget);
        hlabel->setObjectName(QStringLiteral("hlabel"));
        hlabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(hlabel);

        heightEdit = new QLineEdit(centralWidget);
        heightEdit->setObjectName(QStringLiteral("heightEdit"));
        sizePolicy3.setHeightForWidth(heightEdit->sizePolicy().hasHeightForWidth());
        heightEdit->setSizePolicy(sizePolicy3);
        heightEdit->setMinimumSize(QSize(40, 24));
        heightEdit->setMaximumSize(QSize(80, 24));
        heightEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(heightEdit);

        saveButton = new QPushButton(centralWidget);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setMinimumSize(QSize(0, 25));
        saveButton->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(saveButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        fpsLabel = new QLabel(centralWidget);
        fpsLabel->setObjectName(QStringLiteral("fpsLabel"));
        fpsLabel->setMinimumSize(QSize(60, 10));
        fpsLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(fpsLabel);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 17));
        menuBoh = new QMenu(menuBar);
        menuBoh->setObjectName(QStringLiteral("menuBoh"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuBoh->menuAction());
        menuBoh->addAction(actionOpen);
        menuBoh->addAction(actionSave);
        menuBoh->addAction(actionRender);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Basic Renderer", 0));
        actionRender->setText(QApplication::translate("MainWindow", "Render", 0));
#ifndef QT_NO_TOOLTIP
        actionRender->setToolTip(QApplication::translate("MainWindow", "Render Image", 0));
#endif // QT_NO_TOOLTIP
        actionRender->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWindow", "Open File", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        renderButton->setText(QApplication::translate("MainWindow", "Render", 0));
        wlabel->setText(QApplication::translate("MainWindow", "w:", 0));
        hlabel->setText(QApplication::translate("MainWindow", "h:", 0));
        saveButton->setText(QApplication::translate("MainWindow", "Save", 0));
        fpsLabel->setText(QString());
        menuBoh->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
