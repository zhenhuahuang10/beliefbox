// -*- Mode: c++ -*-
// copyright (c) 2006 by Christos Dimitrakakis <christos.dimitrakakis@gmail.com>
// $Revision$
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VALUE_ITERATION_H
#define VALUE_ITERATION_H

#include "DiscreteMDP.h"
#include "real.h"
#include <vector>

class ValueIteration
{
public:
    const DiscreteMDP* mdp;
    real gamma;
    int n_states;
    int n_actions;
    std::vector<real> V;
    std::vector<real*> Q;
    std::vector<real> Q_data;
    real Delta;
    real baseline;
    ValueIteration(const DiscreteMDP* mdp, real gamma, real baseline=0.0);
    ~ValueIteration();
    void Reset();
    void ComputeStateValues(real threshold, int max_iter=-1);
    void ComputeStateActionValues(real threshold, int max_iter=-1);
    real getValue (int state, int action);
    real getValue (int state);
};

#endif
