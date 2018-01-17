#ifndef RADIUMENGINE_MESHCONTACTMANAGER_HPP
#define RADIUMENGINE_MESHCONTACTMANAGER_HPP

#include <Engine/Managers/MeshContactManager/MeshContactElement.hpp>

#include <Engine/System/System.hpp>

#include <Core/TreeStructures/kdtree.hpp>
#include <Core/TreeStructures/trianglekdtree.hpp>

#include <Core/Mesh/ProgressiveMesh/PriorityQueue.hpp>

#include <Core/Mesh/Wrapper/Convert.hpp>

#include <Core/Mesh/ProgressiveMesh/ProgressiveMeshData.hpp>

#include <Core/Mesh/ProgressiveMesh/ProgressiveMesh.hpp>

#include <Core/Geometry/Distance/DistanceQueries.hpp>

#define NBMAX_ELEMENTS 10

#define NBMAX_STEP 256


namespace Ra
{
    namespace Engine
    {

        class MeshContactManager
        {
            struct PtDistrib
            {
                Scalar r;
                Scalar a;
                Ra::Core::Index faceId;
                Ra::Core::Index objId;
            };

        public:

            MeshContactManager();
            void setNbFacesChanged(const int nb);
            void setNbObjectsChanged(const int nb);
            void setThresholdChanged(const double threshold);
            void setLambdaChanged(const double lambda);
            void setMChanged(const double m);
            void setNChanged(const double n);
            void setInfluenceChanged(const double influence);
            void setAsymmetryChanged(const double asymmetry);
            void setLodValueChanged(int value);
            void setComputeR();
            void setLoadDistribution(std::string filePath);
            void setDisplayProximities();
            void setDisplayDistribution();
            void setConstructM0();

            int getNbFacesMax();
            void computeNbFacesMax();
            void computeNbFacesMax2();

            void addMesh(MeshContactElement* mesh, const std::string& entityName, const std::string& componentName);

            void computeThreshold();
            void computeThresholdTest();
            void distanceDistribution();
            void compareDistanceDistribution();
            void distanceAsymmetryDistribution();
            void loadDistribution(std::string filePath);
            void sortDistAsymm();
            void displayDistribution();
            void distanceAsymmetryFiles();
            void distanceAsymmetryFile();
            void thresholdComputation();

            void constructPriorityQueues();
            void updatePriorityQueue(Ra::Core::Index vsIndex, Ra::Core::Index vtIndex, int objIndex);
            bool edgeErrorComputation(Ra::Core::HalfEdge_ptr h, int objIndex, Scalar& error, Ra::Core::Vector3& p);
            void constructPriorityQueues2();
            void updatePriorityQueue2(Ra::Core::Index vsIndex, Ra::Core::Index vtIndex, int objIndex);
            bool edgeCollapse(int objIndex);

        private:

            int m_nb_faces_max;
            int m_nbfacesinit;
            int m_nbfaces;
            int m_nbobjects; // number of objects to be simplified (the first ones to be loaded into the scene)
            Scalar m_threshold; // distance used to define "contacts"
            Scalar m_influence; // weight value for the threshold in order to compute a broader threshold
            Scalar m_asymmetry; // threshold for the asymmetry between the 2 triangles defining a contact
            Scalar m_broader_threshold; // amplified threshold
            Scalar m_lambda; // influence of the original quadric considering contacts
            Scalar m_m;
            Scalar m_n; // slope of the weight function for contacts

            Eigen::Matrix<Scalar, NBMAX_ELEMENTS, NBMAX_ELEMENTS> m_thresholds; // thresholds for each pair of objects
            std::vector<std::vector<std::vector<std::pair<Ra::Core::Index,Scalar> > > > m_distances; // distances for each pair of objects

//            struct comparePtDistribByDistance
//            {
//                inline bool operator() (const PtDistrib &p1, const PtDistrib &p2) const
//                {
//                    return p1.r <= p2.r;
//                }
//            };
//            typedef std::set<PtDistrib, comparePtDistribByDistance> DistanceSorting;
//            DistanceSorting m_distSort;
//            Ra::Core::Index m_curr_r;

//            struct comparePtDistribByAsymmetry
//            {
//                inline bool operator() (const PtDistrib &p1, const PtDistrib &p2) const
//                {
//                    return p1.a < p2.a;
//                }
//            };
//            typedef std::set<PtDistrib, comparePtDistribByAsymmetry> AsymmetrySorting;
//            AsymmetrySorting m_asymmSort;
//            Ra::Core::Index m_curr_a;

            std::vector<PtDistrib> m_distrib;
            Scalar m_threshold_max;
            Scalar m_asymmetry_max;

            std::vector<Super4PCS::TriangleKdTree<>*> m_trianglekdtrees;
            std::vector<MeshContactElement*> m_meshContactElements;

            std::vector<Ra::Core::TriangleMesh> m_initTriangleMeshes; // used for contact computation

            struct comparePriorityQueueDataByErrorContact
            {
                typedef Ra::Core::PriorityQueue::PriorityQueueData PriorityQueueData;
                inline bool operator()(const PriorityQueueData &a1, const PriorityQueueData &a2) const
                {
                    if (a1.m_err != a2.m_err)
                        return a1.m_err < a2.m_err;
                    else
                    {
                        if (a1.m_index != a2.m_index)
                               return (a1.m_index < a2.m_index);
                        if (a1.m_fl_id != a2.m_fl_id)
                            return a1.m_fl_id < a2.m_fl_id;
                        else
                            return a1.m_vs_id + a1.m_vt_id < a2.m_vs_id + a2.m_vt_id;
                    }
                }
            };

            typedef std::set<Ra::Core::PriorityQueue::PriorityQueueData, comparePriorityQueueDataByErrorContact> QueueContact;
            QueueContact m_mainqueue;

            std::vector<int> m_index_pmdata; // array recording in which objects collapses have been done
            int m_curr_vsplit; // index for m_index_pmdata
        };

    } // namespace Engine
} // namespace Ra

#endif // RADIUMENGINE_MESHCONTACTMANAGER_HPP