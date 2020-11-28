#ifndef MEC_H
#define MEC_H

#include <vector>
#include <cmath>
#include <cassert>




//-------------------------------------------------------------------------------------------------------------------------------------------------//
// Implementation of
// Maximum Entropy Coordinates for Arbitrary Polytopes, K. Hormann 1 and N. Sukumar 2, SGP 2008
//
// Warning: this implementation comes with no guarantee, and has not been approved by the authors of the original article. Use at your own risks.
//-------------------------------------------------------------------------------------------------------------------------------------------------//




namespace MEC{
struct Stats{
    unsigned int it;
    bool isNaN;
    double linearPrecisionError;

    Stats() : it(0) , isNaN(false) , linearPrecisionError(0.0) {}
};

double square(double x) { return x*x; }

template< class mat_t , class point_t , class float_t , class float_t2 >
Stats computeCoordinates(
        point_t eta ,
        std::vector< point_t > const & cage_vertices_in ,
        std::vector< float_t > const & i_masses,
        std::vector< float_t2 > & weights ,
        unsigned int max_iterations = 20 , // why not...
        unsigned int nbStepsForLineSearch = 10000 , // why not...
        double maxDW = 0.001 ) // why not...
{
    assert( cage_vertices_in.size() == i_masses.size()   &&   "You have to provide masses w.r.t. the cage vertices" );
    unsigned int n_vertices = cage_vertices_in.size();
    point_t lambda(0,0,0); // initialization
    double epsilonTermination = 0.0000001; // why not
    double errorTrigerringLineSearch = 0.0001; // why not

    Stats mecStats;

    for(  ; mecStats.it < max_iterations ; ++mecStats.it ) {
        point_t gZ(0,0,0);
        mat_t HZ(0,0,0,0,0,0,0,0,0);
        double Z = 0.0;
        for( unsigned int i = 0 ; i < n_vertices ; ++i ) {
            point_t vi_bar = cage_vertices_in[i] - eta;
            double Zi = i_masses[i] * exp( - point_t::dot(lambda , vi_bar) );
            Z += Zi;
            gZ += - Zi * vi_bar;
            HZ += Zi * jm::mat33d::tensor( vi_bar , vi_bar );
        }
        point_t gF = gZ / Z;
        mat_t HF = HZ / Z - jm::mat33d::tensor( gF , gF );

        if( gZ.isnan() ) {
            std::cout << "\t gZ is nan (" << mecStats.it << " iterations, Z = " << Z << ")" << std::endl;
        }
        if( HZ.isnan() ) {
            std::cout << "\t HZ is nan (" << mecStats.it << " iterations, Z = " << Z << ")" << std::endl;
        }
        if( gF.isnan() ) {
            std::cout << "\t gF is nan (" << mecStats.it << " iterations, Z = " << Z << ")" << std::endl;
        }
        if( HF.isnan() ) {
            std::cout << "\t HF is nan (" << mecStats.it << " iterations, Z = " << Z << ")" << std::endl;
        }


        // Newton search direction:
        bool isPseudoInv;
        point_t dLambda = - HF.solveLinearSystem( gF , isPseudoInv );

        // check for a dLambda that is "too big":
        double DW = 0.0;
        for( unsigned int i = 0 ; i < n_vertices ; ++i ) {
            point_t vi_bar = cage_vertices_in[i] - eta;
            DW += i_masses[i]*i_masses[i] * exp( - 2.0 * point_t::dot(lambda , vi_bar) ) * square( exp( - point_t::dot(dLambda , vi_bar) ) - 1 );
        }
        DW = sqrt(DW);
        if( DW > maxDW ) {
            dLambda *= ( maxDW / DW );
        }

        if( dLambda.isnan() ) {
            std::cout << "\t dLambda is nan (" << mecStats.it << " iterations, Z = " << Z << ")" << std::endl;
        }

        double errorCurrent = 0.0;
        for( unsigned int i = 0 ; i < n_vertices ; ++i ) {
            point_t vi_bar = cage_vertices_in[i] - eta;
            double Zi = i_masses[i] * exp( - point_t::dot(lambda + dLambda , vi_bar) );
            errorCurrent += Zi;
        }
        errorCurrent = fabs(log(errorCurrent));

        if(  std::isnan(errorCurrent)  ||  errorCurrent > errorTrigerringLineSearch) {
            double minError = FLT_MAX;
            point_t nextLambda;
            for( unsigned int step = 1 ; step < nbStepsForLineSearch ; ++step ) {
                double s = (double)(step) / (double)(nbStepsForLineSearch);
                point_t lambdaCurrent = lambda + s * dLambda;
                errorCurrent = 0.0;
                for( unsigned int i = 0 ; i < n_vertices ; ++i ) {
                    point_t vi_bar = cage_vertices_in[i] - eta;
                    double Zi = i_masses[i] * exp( - point_t::dot(lambdaCurrent , vi_bar) );
                    errorCurrent += Zi;
                }
                errorCurrent = fabs(log(errorCurrent));
                if( ! std::isnan(errorCurrent)  ){
                    if( minError > errorCurrent ) {
                        errorCurrent = minError;
                        nextLambda = lambdaCurrent;
                    }
                }
            }
            lambda = nextLambda;
            //            if( nextLambda.isnan() ) {
            //                std::cout << "\t 101 : nextLambda is nan (" << it << " iterations, Z = " << Z << ")" << std::endl;
            //            }
        }
        else{
            lambda += dLambda;
            //            if( lambda.isnan() ) {
            //                std::cout << "\t 107 : lambda is nan (" << it << " iterations, Z = " << Z << ")" << std::endl;
            //            }
        }

        if( gF.norm() < epsilonTermination )
            break;
    }

    if(weights.size() != n_vertices) weights.resize(n_vertices);
    double sumW = 0.0;
    for( unsigned int i = 0 ; i < n_vertices ; ++i ) {
        point_t vi_bar = cage_vertices_in[i] - eta;
        double Zi = i_masses[i] * exp( - point_t::dot(lambda , vi_bar) );
        weights[i] = Zi;
        sumW += Zi;

        //        if( std::isnan(Zi) ) {
        //            std::cout << "\t coord " << i << " is nan (" << it << " iterations)" << std::endl;
        //        }
    }

    point_t lprecision(0,0,0);
    for( unsigned int i = 0 ; i < n_vertices ; ++i ) {
        weights[i] /= sumW;
        lprecision += weights[i]*(cage_vertices_in[i] - eta);
        if( std::isnan(weights[i]) ) {
            weights[i] = 0.0;
            mecStats.isNaN = true;
        }
    }
    mecStats.linearPrecisionError = lprecision.norm();
    return mecStats;
}
}



#endif // MEC_H
