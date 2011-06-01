/* -*- Mode: C++; -*- */
/* VER: $Id: Distribution.h,v 1.3 2006/11/06 15:48:53 cdimitrakakis Exp cdimitrakakis $*/
// copyright (c) 2006 by Christos Dimitrakakis <christos.dimitrakakis@gmail.com>
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef DIRICHLET_H
#define DIRICHLET_H

#include "Distribution.h"
#include "MultinomialDistribution.h"

/** Dirichlet distribution.
	
	This distribution is conjugate to the multinomial.
 */
class DirichletDistribution : public VectorDistribution
{
protected:
    int n; ///< size of multinomial distribution
    Vector a; ///< size of vector
public:
    DirichletDistribution();
    DirichletDistribution(int n, real p = 1.0);
    DirichletDistribution(const Vector& x);
    virtual ~DirichletDistribution();
    virtual void generate(Vector& x) const;
    virtual Vector generate() const;
    virtual real pdf(const Vector& x) const;
    virtual real log_pdf(const Vector& x) const;
    virtual void update(Vector* x)
	{
		a += *x;
	}
    /// When there is only one observation, give it directly.
    real Observe(int i);
    Vector GetParameters() const;
    Vector GetMean() const;
    real& Alpha(int i)
    {
        return a[i];
    }
    int size() const
    {
        return n;
    }
    void resize(int n, real p = 0.0);
    
};


#endif

