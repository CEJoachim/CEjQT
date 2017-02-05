#define QT_NO_KEYWORDS
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include "scripteditor.hpp"

ScriptEditor::ScriptEditor(QWidget * parent) : QTextEdit(parent) {
	
}

ScriptEditor::~ScriptEditor() {
	
}

void ScriptEditor::keyPressEvent(QKeyEvent *e)
{
	if(e->modifiers() & Qt::ControlModifier)
	{ 
		if (e->key() == Qt::Key_Return)
		{
			//qDebug() << "KEY OVERRIDE";

			Q_EMIT this->execute();
		}
		else
		{
			QTextEdit::keyPressEvent(e);
		}
	}
	else
	{
		QTextEdit::keyPressEvent(e);
	}
	
}
