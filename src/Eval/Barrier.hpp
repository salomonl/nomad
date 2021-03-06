/*---------------------------------------------------------------------------------*/
/*  NOMAD - Nonlinear Optimization by Mesh Adaptive Direct Search -                */
/*                                                                                 */
/*  NOMAD - Version 4.0.0 has been created by                                      */
/*                 Viviane Rochon Montplaisir  - Polytechnique Montreal            */
/*                 Christophe Tribes           - Polytechnique Montreal            */
/*                                                                                 */
/*  The copyright of NOMAD - version 4.0.0 is owned by                             */
/*                 Charles Audet               - Polytechnique Montreal            */
/*                 Sebastien Le Digabel        - Polytechnique Montreal            */
/*                 Viviane Rochon Montplaisir  - Polytechnique Montreal            */
/*                 Christophe Tribes           - Polytechnique Montreal            */
/*                                                                                 */
/*  NOMAD v4 has been funded by Rio Tinto, Hydro-Québec, NSERC (Natural            */
/*  Sciences and Engineering Research Council of Canada), InnovÉÉ (Innovation      */
/*  en Énergie Électrique) and IVADO (The Institute for Data Valorization)         */
/*                                                                                 */
/*  NOMAD v3 was created and developed by Charles Audet, Sebastien Le Digabel,     */
/*  Christophe Tribes and Viviane Rochon Montplaisir and was funded by AFOSR       */
/*  and Exxon Mobil.                                                               */
/*                                                                                 */
/*  NOMAD v1 and v2 were created and developed by Mark Abramson, Charles Audet,    */
/*  Gilles Couture, and John E. Dennis Jr., and were funded by AFOSR and           */
/*  Exxon Mobil.                                                                   */
/*                                                                                 */
/*  Contact information:                                                           */
/*    Polytechnique Montreal - GERAD                                               */
/*    C.P. 6079, Succ. Centre-ville, Montreal (Quebec) H3C 3A7 Canada              */
/*    e-mail: nomad@gerad.ca                                                       */
/*    phone : 1-514-340-6053 #6928                                                 */
/*    fax   : 1-514-340-5665                                                       */
/*                                                                                 */
/*  This program is free software: you can redistribute it and/or modify it        */
/*  under the terms of the GNU Lesser General Public License as published by       */
/*  the Free Software Foundation, either version 3 of the License, or (at your     */
/*  option) any later version.                                                     */
/*                                                                                 */
/*  This program is distributed in the hope that it will be useful, but WITHOUT    */
/*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or          */
/*  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License    */
/*  for more details.                                                              */
/*                                                                                 */
/*  You should have received a copy of the GNU Lesser General Public License       */
/*  along with this program. If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                                 */
/*  You can find information on the NOMAD software at www.gerad.ca/nomad           */
/*---------------------------------------------------------------------------------*/
#ifndef __NOMAD400_BARRIER__
#define __NOMAD400_BARRIER__

#include "../Eval/EvalPoint.hpp"
#include "../Math/Double.hpp"

#include "../nomad_nsbegin.hpp"

/// Class for barrier following algorithm 12.2 of DFBO.
class Barrier
{
private:
    
    std::vector<EvalPoint> _xFeas;  ///< Current feasible incumbent solutions
    std::vector<EvalPoint> _xInf;   ///< Current infeasible incumbent solutions

    EvalPointPtr _refBestFeas;      ///< Previous first feasible incumbent
    EvalPointPtr _refBestInf;       ///< Previous first infeasible incumbent

    Double _hMax;                   ///< Maximum acceptable value for h

public:
    /// Constructor
    /**
     * hMax will be updated during optimization.
     \param hMax            The max of h to keep a point in the barrier -- \b IN.
     \param fixedVariable   The fixed variables have a fixed value -- \b IN.
     \param evalType        Type of evaluation (BB or SGTE) -- \b IN.
     \param evalPointList   Additional points to consider in building the barrier -- \b IN.
     */
    Barrier(const Double& hMax = INF,
            const Point& fixedVariable = Point(),
            const EvalType& evalType = EvalType::BB,
            const std::vector<EvalPoint> evalPointList = std::vector<EvalPoint>())
      : _xFeas(),
        _xInf(),
        _refBestFeas(nullptr),
        _refBestInf(nullptr),
        _hMax(hMax)
    {
        init(fixedVariable, evalType, evalPointList);
    }
    
    /*-----------------*/
    /* Feasible points */
    /*-----------------*/
    /// Get all feasible points in the barrier
    /**
     \return All the eval points that are feasible.
     */
    const std::vector<EvalPoint> getAllXFeas()    const { return _xFeas; }

    /// Update ref best feasible and ref best infeasible values.
    void updateRefBests();
    
    ///  Get the first feasible point in the barrier.
    /**
     * If there is no feasible point, return a \c nullptr
     \return A single feasible eval point.
     */
    EvalPointPtr getFirstXFeas() const;
    
    ///  Get the point that was previously the first feasible point in the barrier.
    /**
     * If there is no feasible point, return a \c nullptr
     \return A single feasible eval point.
     */
    EvalPointPtr getRefBestFeas() const { return _refBestFeas; }

    /// Number of feasible points in the barrier.
    size_t nbXFeas() const { return _xFeas.size(); }
    
    /// Add a feasible point in the barrier.
    /**
     * If the point is feasible it is added, if not an exception is triggered.
     \param xFeas       The eval point to add -- \b IN.
     \param evalType    Which eval (Blackbox or Surrogate) of the EvalPoint to use to verify feasibility  -- \b IN.
     */
    void addXFeas(const EvalPoint &xFeas, const EvalType& evalType);
    
    /// Remove feasible points from the barrier.
    void clearXFeas();
    
    /*-------------------*/
    /* Infeasible points */
    /*-------------------*/
    ///  Get all infeasible points in the barrier
    /**
     \return All the eval points that are infeasible.
     */
    const std::vector<EvalPoint> getAllXInf() const { return _xInf; }
    
    ///  Get the first infeasible point in the barrier.
    /**
     * If there is no infeasible point, return a \c nullptr
     \return A single infeasible eval point.
     */
    EvalPointPtr getFirstXInf() const;
    
    ///  Get the point that was previously the first infeasible point in the barrier.
    /**
     * If there is no feasible point, return a \c nullptr
     \return A single feasible eval point.
     */
    EvalPointPtr getRefBestInf() const { return _refBestInf; }
    //void setRefBestInf(const EvalPointPtr refBestInf) { _refBestInf = refBestInf; }

    /// Number of infeasible points in the barrier.
    size_t nbXInf() const { return _xInf.size(); }
    
    /// Add an infeasible point in the barrier.
    /**
     * If the point is nullptr an exception is triggered.
     \param xInf   The eval point to add -- \b IN.
     */
    void addXInf(const EvalPoint &xInf);
    
    /// Remove infeasible points from the barrier.
    void clearXInf();

    /*---------------*/
    /* Other methods */
    /*---------------*/
    /// Get all feasible and infeasable points
    std::vector<EvalPoint> getAllPoints();

    /// Get the current hMax of the barrier.
    Double getHMax() const { return _hMax; }
    
    /// Set the hMax of the barrier
    /**
     \param hMax    The hMax -- \b IN.
     */
    void setHMax(const Double &hMax);

    /// Update xFeas and xInf according to given points.
    /* \param evalPointList vector of EvalPoints  -- \b IN.
     * \param keepAllPoints keep all good points, or keep just one point as in NOMAD 3 -- \b IN.
     * \return true if the Barrier was updated, false otherwise
     * \note Input EvalPoints are already in subproblem dimention
     */
    bool updateWithPoints(const std::vector<NOMAD::EvalPoint>& evalPointList,
                          const EvalType& evalType,
                          const bool keepAllPoints);

    /// Return the barrier as a string.
    /* May be used for information, or for saving a barrier. In the former case,
     * it may be useful to set parameter max to a small value (e.g., 4). In the
     * latter case, INF_SIZE_T should be used so that all points are saved.
     * \param max Maximum number of feasible and infeasible points to display
     * \return A string describing the barrier
     */
    std::string display(const size_t max = INF_SIZE_T) const;

private:
    
    /**
     * \brief Helper function for constructor.
     *
     * Will throw exceptions or output error messages if something is wrong. Will remain silent otherwise.
     \param fixedVariable  The fixed variables have a fixed value     -- \b IN.
     \param evalType        Which eval (Blackbox or Surrogate) to use to verify feasibility  -- \b IN.
     \param evalPointList Additional points to consider to construct barrier. -- \b IN.
     */
    void init(const Point& fixedVariable,
              const EvalType& evalType,
              const std::vector<EvalPoint> evalPointList);
    
    /**
     * \brief Helper function for init/constructor.
     *
     * Throw an exception if the Cache has not been instanciated yet. Will remain silent otherwise.
     */
    void checkCache();

    /**
     * \brief Helper function for insertion.
     *
     * Will throw exceptions or output error messages if something is wrong. Will remain silent otherwise.
     */
    void checkXFeas(const EvalType& evalType);
    
    /**
     * \brief Helper function for insertion.
     *
     * Will throw exceptions or output error messages if something is wrong. Will remain silent otherwise.
     */
    void checkXFeasIsFeas(const EvalType& evalType);
    
    /**
     * \brief Helper function for insertion.
     *
     * Will throw exceptions or output error messages if something is wrong. Will remain silent otherwise.
     */
    void checkXInf();
    
    /**
     * \brief Helper function for init/setHMax.
     *
     * Will throw exceptions or output error messages if something is wrong. Will remain silent otherwise.
     */
    void checkHMax();
};


/// Display useful values so that a new Barrier could be constructed using these values.
std::ostream& operator<<(std::ostream& os, const Barrier& barrier);

/// Get barrier values from stream
std::istream& operator>>(std::istream& is, Barrier& barrier);


#include "../nomad_nsend.hpp"

#endif // __NOMAD400_BARRIER__
