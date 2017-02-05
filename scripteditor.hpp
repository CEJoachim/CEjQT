#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H
#define QT_NO_KEYWORDS
#include <QTextEdit>

class ScriptEditor : public QTextEdit {
	Q_OBJECT

public:
	ScriptEditor(QWidget * parent = Q_NULLPTR);
	~ScriptEditor();

protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

Q_SIGNALS:
	void *execute();


private:
	
};


#endif 