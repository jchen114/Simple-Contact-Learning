#pragma once
#include <Python.h>
#include <vector>
#include "State.h"
#include "Action.h"
#include <algorithm>

class TheanoRLManager
{
public:
	TheanoRLManager(int input_dimension, std::vector<int> hidden_units, int output_units, float gamma=0.9f, float regularization_factor=0.0f);
	~TheanoRLManager();

	void SendBatchForTraining(std::vector<std::tuple<State, Action, float, State>> batch);
	void QueryQvalues(State current_state);

private:

	PyObject *m_NNBuilderObj;

	PyObject *MakeList(std::vector<int> args);

	void AddObjectForTracking(PyObject *object);
	void DecReference(PyObject *object);

	void ClearObjects();

	std::vector<PyObject *> m_pyObjects;

};

static const std::string RL_PATH = "../../Dependencies/RL-Theano/NNBuilder";
static const std::string MOD_NN_BUILDER_NAME = "NNBuilder";