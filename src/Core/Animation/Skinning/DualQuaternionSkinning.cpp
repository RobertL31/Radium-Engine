#include <Core/Animation/Skinning/DualQuaternionSkinning.hpp>

namespace Ra {
namespace Core {
namespace Animation {

void computeDQ( const Pose& pose, const WeightMatrix& weight, DQList& DQ ) {
    CORE_ASSERT( ( pose.size() == weight.cols() ), "pose/weight size mismatch." );
    DQ.clear();
    DQ.resize( weight.rows(), DualQuaternion( Quaternion( 0, 0, 0, 0 ), Quaternion( 0, 0, 0, 0 ) ) );
    // Loop through the weights in a pose-element-wise way
    for( int k = 0; k < weight.outerSize(); ++k ) {
        const DualQuaternion q( pose[k] );
        // Count how many vertices are influenced by the given transform
        const int nonZero = weight.col( k ).nonZeros();
#if defined CORE_USE_OMP
        omp_set_dynamic(0);
        #pragma omp parallel for schedule( static ) num_threads(4)
#endif
        // This for loop is here just because OpenMP wants classic for loops.
        // Since we cannot iterate directly through the non-zero elements using the InnerIterator,
        // we inizialize an InnerIterator to the first element and then we increase it nz times.
        /*
        * This crappy piece of code was done in order to avoid the critical section
        *           DQ[i] += wq;
        *
        * that was occurring when parallelizing the main for loop.
        *
        * NOTE: this could be definitely improved by using std::thread
        */
        for( int nz = 0; nz < nonZero; ++nz ) {
            WeightMatrix::InnerIterator it( weight, k );
            for( int j = 0; j < nz; ++j ) {
                ++it;
            }
            const uint   i  = it.row();
            const Scalar w  = it.value();
            const auto   wq = q * w;
            DQ[i] += wq;
        }
    }

    // Normalize all dual quats.
#if defined CORE_USE_OMP
    omp_set_dynamic(0);
    #pragma omp parallel for schedule( static ) num_threads(4)
#endif
    for( int i = 0; i < int(DQ.size()) ; ++i) {
        DQ[i].normalize();
    }
}

void dualQuaternionSkinning( const Vector3Array& input, const DQList& DQ, Vector3Array& output ) {
    const uint size = input.size();
    CORE_ASSERT( ( size == DQ.size() ), "input/DQ size mismatch." );
    output.resize( size );
#if defined CORE_USE_OMP
    omp_set_dynamic(0);
    #pragma omp parallel for schedule( static ) num_threads(4)
#endif
    for( int i = 0; i < int(size); ++i ) {
        output[i] = DQ[i].transform( input[i] );
    }
}

} // namespace Animation
} // namespace Core
} // namespace Ra
