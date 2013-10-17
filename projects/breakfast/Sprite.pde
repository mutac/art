
import gifAnimation.*;

/**
 */
interface ISprite {
  boolean collidesWith(ISprite s);
  boolean exceeds(PVector minExtent, PVector maxExtent);
  void draw();
  void reset();
};

/**
 */
class ImageSprite implements ISprite {
  PImage _sprite;
  IVectorGenerator _animator;
  PVector _location;
  float _spriteRadius;
  
  ImageSprite(PImage sprite, IVectorGenerator animator) {
    _sprite = sprite;
    _animator = animator;
    _location = _animator.next();
    _spriteRadius = max(_sprite.width, _sprite.height);
  }
  
  void draw() {  
    image(_sprite, _location.x, _location.y);
    _location = _animator.next();
  }
  
  void reset() {
    _animator.reset();
  }
  
  boolean collidesWith(ISprite s) {
    return false;
  }
  
  boolean exceeds(PVector minExtent, PVector maxExtent) {
    return VectorMethods.exceeds(_location, minExtent, maxExtent);
  }
};

/**
 */
class GifSprite implements ISprite {
  Gif _sprite;
  IVectorGenerator _animator;
  PVector _location;
  float _spriteRadius;
  
  GifSprite(Gif sprite, IVectorGenerator animator) {
    _sprite = sprite;
    _animator = animator;
    _location = _animator.next();
    _spriteRadius = max(_sprite.width, _sprite.height);
  }
  
  void draw() {  
    image(_sprite, _location.x, _location.y);
    _location = _animator.next();
  }
  
  void reset() {
    _animator.reset();
  }
  
  boolean collidesWith(ISprite s) {
    return false;
  }
  
  boolean exceeds(PVector minExtent, PVector maxExtent) {
    return VectorMethods.exceeds(_location, minExtent, maxExtent);
  }
};

/**
 */
class SpriteArena implements ISprite {
  private ArrayList _sprites;
  PVector _minBounds;
  PVector _maxBounds;
  
  SpriteArena(PVector minBounds, PVector maxBounds) {
    _sprites = new ArrayList();
    _minBounds = minBounds;
    _maxBounds = maxBounds;
  }
  
  void add(ISprite sprite) {
    _sprites.add(sprite);
  }
  
  void draw() {
    for (int spriteIdx = 0; spriteIdx < _sprites.size(); spriteIdx++) {
      ISprite sprite = (ISprite) _sprites.get(spriteIdx);
      sprite.draw();
      
      if (sprite.exceeds(_minBounds, _maxBounds)) {
        sprite.reset();
      }
    }
  }
  
  void reset() {
    for (int spriteIdx = 0; spriteIdx < _sprites.size(); spriteIdx++) {
      ISprite sprite = (ISprite) _sprites.get(spriteIdx);
      sprite.reset();
    }
  }
  
  boolean collidesWith(ISprite s) {
    for (int spriteIdx = 0; spriteIdx < _sprites.size(); spriteIdx++) {
      ISprite sprite = (ISprite) _sprites.get(spriteIdx);
      if (sprite.collidesWith(s))
      {
        return true;
      }
    }
    return false;
  }
  
  boolean exceeds(PVector minExtent, PVector maxExtent) {
    for (int spriteIdx = 0; spriteIdx < _sprites.size(); spriteIdx++) {
      ISprite sprite = (ISprite) _sprites.get(spriteIdx);
      if (sprite.exceeds(minExtent, maxExtent))
      {
        return true;
      }
    }
    return false;
  }
};

/**
 */
class PVectorMethods {
  // Can't make methods static in processing...
  boolean collidesWith(PVector pointA, float radiusA, PVector pointB, float radiusB) {
    float distance = pointA.dist(pointB);
    return (distance < (radiusA + radiusB));
  }
  
  // Can't make methods static in processing...
  boolean exceeds(PVector pointA, PVector minExtent, PVector maxExtent) {
    if (pointA.x < minExtent.x || pointA.y < minExtent.y || pointA.z < minExtent.z) {
      return true;
    } else if (pointA.x > maxExtent.x || pointA.y > maxExtent.y || pointA.z > maxExtent.z) {
      return true;
    } else {
      return false;
    }     
  }
};

// Can't make methods static in processing...
PVectorMethods VectorMethods = new PVectorMethods();
