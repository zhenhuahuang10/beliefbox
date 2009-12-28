/* -*- Mode: c++;  -*- */
// copyright (c) 2009 by Christos Dimitrakakis <christos.dimitrakakis@gmail.com>
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef BAYESIAN_PSR_H
#define BAYESIAN_PSR_H

#include <vector>
#include <map>
#include "Vector.h"
#include "Matrix.h"

/**
   \ingroup StatisticsGroup
 */
/*@{*/


typedef std::map<int, real, std::greater<int> > BeliefMap;
typedef BeliefMap::iterator BeliefMapIterator;

/*** A factored Markov Chain, basically.
 */
class BayesianPredictiveStateRepresentation
{
protected:
    Matrix P_obs; ///< Probability of observations for model k
    Matrix Lkoi; ///< Probability of observations for all models up to k
    std::vector<real> weight; ///< temporary weight of model
public:
    std::vector<BeliefMap> beliefs;

    BayesianPredictiveStateRepresentation (int n_obs, int n_models, float prior, bool dense = false);

    inline real get_belief_param(int model)
    {
        int src = mc[model]->getCurrentState();
        BeliefMapIterator i = beliefs[model].find(src);
		if (i==beliefs[model].end()) {
			return 0.0;
		} else {
			return i->second;
		}
    }

    inline void set_belief_param(int model, real value)
    {
        int src = mc[model]->getCurrentState();
        BeliefMapIterator i =  beliefs[model].find(src);
        if (i!=beliefs[model].end()) {
            i->second = value;
        } else {
            beliefs[model].insert(std::make_pair(src, value));
        }
    }

    virtual ~BayesianPredictiveStateRepresentation();

    
    /* Training and generation */
    virtual void ObserveNextState (int state);
    inline void Observe(int x) {
        ObserveNextState(x);
    }
    virtual real NextStateProbability (int state);
    virtual void Reset();
    virtual int predict();
    
};
/*@}*/
#endif
