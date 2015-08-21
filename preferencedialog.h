#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QMainWindow>

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit PreferenceDialog(QWidget *parent = 0);
    ~PreferenceDialog();

private:
    Ui::PreferenceDialog *ui;
};

#endif // PREFERENCEDIALOG_H
