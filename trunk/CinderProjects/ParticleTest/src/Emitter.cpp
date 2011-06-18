package graphics;

import java.util.LinkedList;
import java.util.ListIterator;

import javax.media.opengl.GL;

import processing.core.*;
import processing.opengl.PGraphicsOpenGL;

public abstract class Emitter extends Entity
{
  protected PImage     				 particleImage;
  
	private LinkedList<Particle> particles;     // All the particles
	private LinkedList<Modifier> modifiers;     // All the modifiers
	
  private float 							 particlesPerFrame;
  private float 							 particlesPerFrameCount;
  
  private boolean killed;
  
  public enum BlendType_T
  {
  	BlendType_Add,
  	BlendType_Sub,
  	BlendType_None
  };

  private BlendType_T blendType;
  
  public Emitter(PApplet processing, String particleImageFile, float ppf, BlendType_T bt) 
  {
    super (processing);
    
    particles = new LinkedList<Particle>();
    modifiers = new LinkedList<Modifier>();
    
    particleImage = p.loadImage(particleImageFile);
    
    particlesPerFrame = ppf;
    
    killed = false;
    
    blendType = bt;
  }

  // A method to test if the particle system still has particles
  public boolean dead() 
  {
    return killed && particles.isEmpty();
  }

  public void addModifier (Modifier modifier)
  {
  	modifiers.add (modifier);
  }
  
  public int getCount()
  {
  	return particles.size();
  }
  
  public void kill() 
  {
  	killed = true;
  }
  
  @Override
  public void update()
  {
  	particlesPerFrameCount += particlesPerFrame;
  	
  	if (!killed && particlesPerFrameCount >= 1.0f)
  	{
	  	// create and add particles
	    for (int i=(int)particlesPerFrameCount; i > 0; i--)
	    {
	    	particles.add(createParticle());
	    }
	    
	    particlesPerFrameCount = 0.0f;
  	}
  	
    // Update particles
    ListIterator<Particle> pit = particles.listIterator();
    while (pit.hasNext())
    {
      Particle p = pit.next();

      // Update modifiers
      ListIterator<Modifier> mit = modifiers.listIterator();
      while (mit.hasNext())
      {
        Modifier m = mit.next();
        
        m.apply(p);
      }
      
      p.update();
      
      if (p.dead())
        pit.remove();
    }
    
  }

  @Override
  public void draw()
  {
    GL gl=((PGraphicsOpenGL)p.g).gl;	    
    
    p.beginShape(PConstants.QUADS);
    
		gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE);
		switch (blendType)
		{
			case BlendType_Add:	gl.glBlendEquation(GL.GL_FUNC_ADD);								break; 
			case BlendType_Sub:	gl.glBlendEquation(GL.GL_FUNC_REVERSE_SUBTRACT);	break; 
		}
		
    p.texture(particleImage);
  	
  	// Draw all particles
    ListIterator<Particle> it = particles.listIterator();
    while (it.hasNext())
      it.next().draw();
    
    p.endShape(PConstants.CLOSE);    
  }

  public abstract Particle createParticle(); 

}