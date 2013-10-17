/**
 */
interface ISpriteAnimator extends IVectorGenerator {
  void seed(PVector s);
  PVector next();
  void reset();
};

/**
 */
/*
class LinearSpriteAnimator implements ISpriteAnimator {
  private IVectorGenerator _startLocationGenerator;
  private IVectorGenerator _velocityGenerator;
  private PVector _location;
  private PVector _velocity;
  
  LinearSpriteAnimator(IVectorGenerator startLocation, PVector velocity) {
    _startLocationGenerator = startLocation;
    _location = _startLocationGenerator.next();
    _velocityGenerator = new ConstantVectorGenerator(velocity);
  }
  
  LinearSpriteAnimator(IVectorGenerator startLocation, float slope, float velocityAlongSlope) {
    _startLocationGenerator = startLocation;
    _location = _startLocationGenerator.next();
    
    float theta = atan(slope);
    PVector velocity = new PVector(cos(theta), sin(theta));
    velocity.mult(new PVector(linearVelocity, linearVelocity));
    _velocityGenerator = new ConstantVectorGenerator(velocity);
  }
   
  PVector next() {
    _location.add(_velocityGenerator.next());
    return _location.get();
  }
  
  void reset() {
    _location = _startLocationGenerator.next();
  }
};
*/
