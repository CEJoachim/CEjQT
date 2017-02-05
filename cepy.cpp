#define QT_NO_KEYWORDS
#include "cepy.hpp"
#include <QDebug>
#include <python.h>
#include <QSlider>

static int numargs = 1;

/* Return the number of arguments of the application command line */
PyObject*
CePy::emb_numargs(PyObject *self, PyObject *args)
{
	PyObject *object;
	int a;
	int *b;

	if (!PyArg_ParseTuple(args, "i|i:numargs", a, &b))
		return NULL;
	
	pyCalls();
	printf("a: %i b:%i", int(a),b);
	//glWidget->setLineWidth(int(a));
	//lineSlider->setValue(int(a));
	
	return PyLong_FromLong(numargs);
}

void CePy::pyCalls()
{
	printf("pycall");
}

PyObject*
emb_dog(PyObject *self, PyObject *args)
{
	printf("Hello In C++");
	if (!PyArg_ParseTuple(args, ":dog"))
		return NULL;
	return PyLong_FromLong(numargs);
}

PyMethodDef EmbMethods[] = {
	{ "numargs", CePy::emb_numargs, METH_VARARGS,
	"Return the number of arguments received by the process." },
	{ NULL, NULL, 1, NULL },
	{ "numargs", emb_dog, METH_VARARGS,
	"Return the number of arguments received by the process." },
	{ NULL, NULL, 1, NULL },
};

PyModuleDef EmbModule = {
	PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
	NULL, NULL, NULL, NULL
};

PyObject*
PyInit_emb(void)
{
	return PyModule_Create(&EmbModule);
}

CePy::CePy(int argc, char *argv[])
{
	
	//QObject::connect(this, &CePy::pyCall, this, &CePy::pyCalls);

	program = Py_DecodeLocale(argv[0], NULL);
	if (program == NULL) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);  /* optional but recommended */
	Py_SetPythonHome(L"C:/Anaconda3");

	numargs = argc;
	PyImport_AppendInittab("emb", &PyInit_emb);

	Py_Initialize();
	PyRun_SimpleString("print('CePy: Python Initialized!! :)')");
	PyRun_SimpleString("from time import time,ctime\n"
		"print('CePy:', ctime(time()))\n");

	//PyRun_SimpleString("import pythonCode\n");


	char *module = "pythonCode";
	//char *function = "multiply";
	

	
	
	pName = PyUnicode_DecodeFSDefault(module);

	pModule = PyImport_Import(pName);

	Py_DECREF(pName);

	//pFunc = PyObject_GetAttrString(pModule, function);

	

	
	//pValue = PyObject_CallObject(pFunc, pArgs);
	/*
	Py_DECREF(pArgs);
	if (pValue != NULL) {
		printf("Result of call: %ld\n", PyLong_AsLong(pValue));
		Py_DECREF(pValue);
	}*/
	

}

CePy::~CePy()
{
	Py_Finalize();
	PyMem_RawFree(program);
}

int CePy::runModule(char *module, char *function, char *argv[])
{
	//PyRun_SimpleString("print('CePy: Running Script..')");
	char *pyArgs[2] = { "8","2" };
	
	int argC = sizeof(pyArgs) / sizeof(*pyArgs);
	int i;
	pArgs = PyTuple_New(argC);
	for (i = 0; i < argC; ++i) {
		pValue = PyLong_FromLong(atoi(pyArgs[i]));
		PyTuple_SetItem(pArgs, i, pValue);
	}
	pFunc = PyObject_GetAttrString(pModule, function);
	PyObject_CallObject(pFunc, pArgs);

	return 1;
}

int CePy::execScript(const char *script)
{
	PyRun_SimpleString(script);
	return 1;
}