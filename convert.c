#include <Python.h>

#include "interface.h"
#include "command.h"
#include "download.h"

extern void about(void);
extern void help(void);

static PyObject* convert_intf_init(PyObject *self, PyObject *args)
{
    char *intf_type;
    char *dev;

    if(!(PyArg_ParseTuple(args, "zz", &intf_type, &dev))){
        return NULL;
    }

    int ret = intf_init(intf_type, dev);

    return Py_BuildValue("i", ret);
}

static PyObject * convert_cmd_init(PyObject *self, PyObject *args)
{
	int ret = cmd_init();

    return Py_BuildValue("i", ret);
}

static PyObject * convert_dl_init(PyObject *self, PyObject *args)
{
	int ret = dl_init();

    return Py_BuildValue("i", ret);
}

static PyObject* convert_dl_flash_fdl(PyObject *self, PyObject *args)
{
    unsigned char *chip;
    unsigned int addr;

    if(!(PyArg_ParseTuple(args, "si", &chip, &addr))){
        return NULL;
    }

    int ret = dl_flash_fdl(chip, addr);

    return Py_BuildValue("i", ret);
}

static PyObject* convert_dl_flash_file(PyObject *self, PyObject *args)
{
    char *fname;
    unsigned int addr;

    if(!(PyArg_ParseTuple(args, "si", &fname, &addr))){
        return NULL;
    }

    int ret = dl_flash_file(fname, addr);

    return Py_BuildValue("i", ret);
}

static PyObject * convert_about(PyObject *self, PyObject *args)
{
	about();

    return Py_BuildValue("");
}

static PyObject * convert_help(PyObject *self, PyObject *args)
{
	help();

    return Py_BuildValue("");
}

static PyMethodDef uwpflash_methods[] = {
    {"intf_init", convert_intf_init, METH_VARARGS},
    {"cmd_init", convert_cmd_init, METH_VARARGS},
    {"dl_init", convert_dl_init, METH_VARARGS},
    {"dl_flash_fdl", convert_dl_flash_fdl, METH_VARARGS},
    {"dl_flash_file", convert_dl_flash_file, METH_VARARGS},
    {"about", convert_about, METH_VARARGS},
    {"help", convert_help, METH_VARARGS},
    {NULL, NULL},
};

void inituwpflash(void)
{
    Py_InitModule("uwpflash", uwpflash_methods);
}
