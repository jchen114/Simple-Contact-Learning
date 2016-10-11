#include "stdafx.h"
#include "TheanoRLManager.h"

TheanoRLManager::TheanoRLManager(int input_dimension, std::vector<int> hidden_units, int output_units, float gamma = 0.9f, float regularization_factor = 0.0f)
{
	Py_Initialize();
	PySys_SetPath((char *)RL_PATH.c_str());
	m_NNBuilderObj = PyImport_ImportModule(MOD_NN_BUILDER_NAME.c_str());

	// Test for response
	
}


TheanoRLManager::~TheanoRLManager()
{
}


#pragma region UTILS

PyObject *TheanoRLManager::MakeList(std::vector<int> args) {
	PyObject *list = PyList_New(args.size());

	for (std::vector<int>::iterator it = args.begin(); it != args.end(); ++it) {
		PyList_SetItem(list, sizeof(*it), PyInt_FromLong(*it));
	}
	return list;
}

#pragma endregion UTILS

#pragma region JANITOR

void TheanoRLManager::AddObjectForTracking(PyObject *object) {
	m_pyObjects.push_back(object);
}

void TheanoRLManager::DecReference(PyObject *object) {

	Py_DecRef(object);
	m_pyObjects.erase(std::remove(m_pyObjects.begin(), m_pyObjects.end(), object), m_pyObjects.end());

}

void TheanoRLManager::ClearObjects() {
}

#pragma endregion JANITOR