/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
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
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(660, 463);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(5);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionRender = new QAction(MainWindow);
        actionRender->setObjectName(QString::fromUtf8("actionRender"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMinimumSize(QSize(330, 240));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        renderWidget = new QRenderingWidget(centralWidget);
        renderWidget->setObjectName(QString::fromUtf8("renderWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(renderWidget->sizePolicy().hasHeightForWidth());
        renderWidget->setSizePolicy(sizePolicy2);
        renderWidget->setMinimumSize(QSize(640, 360));

        gridLayout->addWidget(renderWidget, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        renderButton = new QPushButton(centralWidget);
        renderButton->setObjectName(QString::fromUtf8("renderButton"));
        renderButton->setMinimumSize(QSize(0, 25));
        renderButton->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(renderButton);

        rendererComboBox = new QComboBox(centralWidget);
        rendererComboBox->setObjectName(QString::fromUtf8("rendererComboBox"));
        rendererComboBox->setMinimumSize(QSize(0, 24));
        rendererComboBox->setMaximumSize(QSize(16777215, 24));

        horizontalLayout->addWidget(rendererComboBox);

        shadingComboBox = new QComboBox(centralWidget);
        shadingComboBox->setObjectName(QString::fromUtf8("shadingComboBox"));
        shadingComboBox->setMinimumSize(QSize(0, 24));
        shadingComboBox->setMaximumSize(QSize(16777215, 24));

        horizontalLayout->addWidget(shadingComboBox);

        wlabel = new QLabel(centralWidget);
        wlabel->setObjectName(QString::fromUtf8("wlabel"));
        wlabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(wlabel);

        widthEdit = new QLineEdit(centralWidget);
        widthEdit->setObjectName(QString::fromUtf8("widthEdit"));
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
        hlabel->setObjectName(QString::fromUtf8("hlabel"));
        hlabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(hlabel);

        heightEdit = new QLineEdit(centralWidget);
        heightEdit->setObjectName(QString::fromUtf8("heightEdit"));
        sizePolicy3.setHeightForWidth(heightEdit->sizePolicy().hasHeightForWidth());
        heightEdit->setSizePolicy(sizePolicy3);
        heightEdit->setMinimumSize(QSize(40, 24));
        heightEdit->setMaximumSize(QSize(80, 24));
        heightEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(heightEdit);

        saveButton = new QPushButton(centralWidget);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setMinimumSize(QSize(0, 25));
        saveButton->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(saveButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        fpsLabel = new QLabel(centralWidget);
        fpsLabel->setObjectName(QString::fromUtf8("fpsLabel"));
        fpsLabel->setMinimumSize(QSize(60, 10));
        fpsLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(fpsLabel);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 660, 21));
        menuBoh = new QMenu(menuBar);
        menuBoh->setObjectName(QString::fromUtf8("menuBoh"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Basic Renderer", nullptr));
        actionRender->setText(QCoreApplication::translate("MainWindow", "Render", nullptr));
#if QT_CONFIG(tooltip)
        actionRender->setToolTip(QCoreApplication::translate("MainWindow", "Render Image", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionRender->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen->setToolTip(QCoreApplication::translate("MainWindow", "Open File", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        renderButton->setText(QCoreApplication::translate("MainWindow", "Render", nullptr));
        wlabel->setText(QCoreApplication::translate("MainWindow", "w:", nullptr));
        hlabel->setText(QCoreApplication::translate("MainWindow", "h:", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        fpsLabel->setText(QString());
        menuBoh->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
