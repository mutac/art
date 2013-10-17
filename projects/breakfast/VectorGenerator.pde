/**
 */
interface IVectorGenerator {
  void seed(PVector s);
  PVector next(); 
  void reset();
};

/**
 */
class ConstantVectorGenerator implements IVectorGenerator {
  private PVector _v;
  
  ConstantVectorGenerator() {
    _v = new PVector(0, 0);
  }
  
  ConstantVectorGenerator(PVector v) {
    _v = v.get();
  }
  
  ConstantVectorGenerator(float angleRad) {
   _v = new PVector(cos(angleRad), sin(angleRad));
  }
  
  ConstantVectorGenerator(float angleRad, float magnitude) {
   _v = new PVector(cos(angleRad), sin(angleRad));
   _v.mult(magnitude);
  }
  
  void seed(PVector s) {
    _v = s.get();
  }
  
  PVector next() {
    return _v.get();
  }
  
  void reset() {
  }
};

/**
 */
class RandomVectorGenerator implements IVectorGenerator {
  private PVector _minBounds;
  private PVector _maxBounds;
  
  RandomVectorGenerator(PVector minBounds, PVector maxBounds) {
    _minBounds = minBounds.get();
    _maxBounds = maxBounds.get();
  }
  
  void seed(PVector s) {
    randomSeed(0);
  }
  
  PVector next() {
    float rX = random(_minBounds.x, _maxBounds.x);
    float rY = random(_minBounds.y, _maxBounds.y);
    float rZ = random(_minBounds.z, _maxBounds.z);
    return new PVector(rX, rY, rZ);
  }
  
  void reset() {
  }
};

/**
 */
class OscillationgVectorGenerator implements IVectorGenerator {
  private float _angleStep = TWO_PI / 25.0;
  private float _magnitude;
  private float _angle;
  
  OscillationgVectorGenerator(float magnitude) {
    _angle = 0.0;
    _magnitude = magnitude;
  }
  
  void seed(PVector s) {
    if (s.x > 0.0) {
      _angle = atan(s.y / s.x);
    } else {
      reset();
    }
  }
  
  PVector next() {
    //_angle = norm(_angle + _angleStep, 0, TWO_PI);
    _angle = _angle + _angleStep;
    PVector v = new PVector(cos(_angle), sin(_angle));
    v.mult(_magnitude);
    return v;
  }
  
  void reset() {
    _angle = 0.0;
  }
};

/**
 */
class MotionVectorGenerator implements IVectorGenerator {
  private IVectorGenerator _velocityGenerator;
  private PVector _startLocation;
  private PVector _location;
  
  MotionVectorGenerator(IVectorGenerator velocityGenerator) {
    _velocityGenerator = velocityGenerator;
    seed(_velocityGenerator.next());
  }
  
  void seed(PVector s) {
    _startLocation = s.get();
    _location = _startLocation.get();
  }
  
  PVector next() {
    _location.add(_velocityGenerator.next());
    return _location.get();
  }
  
  void reset() {
    _location = _startLocation.get();
  }
};

/**
 */
class PositionedVectorGenerator implements IVectorGenerator {
  private IVectorGenerator _startLocationGenerator;
  private IVectorGenerator _locationGenerator;
  
  PositionedVectorGenerator(IVectorGenerator startLocationGenerator, IVectorGenerator locationGenerator) {
    _startLocationGenerator = startLocationGenerator;
    _locationGenerator = locationGenerator;
    
    reset();
  }
  
  void seed(PVector s) {
    _startLocationGenerator.seed(s);
    reset();
  }
  
  PVector next() {
    return _locationGenerator.next();
  }
  
  void reset() {
    _locationGenerator.seed(_startLocationGenerator.next());
  }
};

