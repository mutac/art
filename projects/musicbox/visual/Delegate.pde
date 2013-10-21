import processing.serial.*;
import java.lang.reflect.*;
import java.util.*;

/**
 */
<T> Class<T> typeUnwrapPrimitive(Class<T> c)
{
  if (TypeWrapperToPrimitives.map.containsKey(c))
  {
    return (Class<T>) TypeWrapperToPrimitives.map.get(c);
  }
  else
  {
    return c;
  }
}

/**
 */
public static class TypeWrapperToPrimitives
{
  public static final Map<Class<?>, Class<?>> map;
  static
  {
    map = new HashMap<Class<?>, Class<?>>();
    map.put(Boolean.class, Boolean.TYPE);
    map.put(Byte.class, Byte.TYPE);
    map.put(Character.class, Character.TYPE);
    map.put(Double.class, Double.TYPE);
    map.put(Float.class, Float.TYPE);
    map.put(Integer.class, Integer.TYPE);
    map.put(Long.class, Long.TYPE);
    map.put(Short.class, Short.TYPE);
    map.put(Void.class, Void.TYPE);
  }
}

/**
 */
public class Delegate
{
  public Delegate(Object obj, String methodName)
  {
    mMethodTable = new HashMap<String, Method>();
    mObj = obj;
    mMethodName = methodName;
  }
  
  public void invoke()
  {
    methodInvoke(new Object [] {});
  }

  public void invoke(Object arg0)
  {
    methodInvoke(new Object [] {arg0});
  }
  
  public boolean equals(Delegate other)
  {
    return mObj == other.mObj && mMethodName == other.mMethodName;
  }

  private void methodInvoke(Object[] argv)
  {
    Class [] argTypes = objectTypes(argv);
    Method meth = getMethod(argTypes);

    try
    {
      if (meth != null)
      {
        meth.invoke(mObj, argv);
      }
    }
    catch (Exception e)
    {
      println("Unable to invoke " + delegateSignatureToString(argTypes));
    }
  }

  private Method getMethod(Class[] argTypes)
  {
    String argHash = typesToString(argTypes);
    
    if (mMethodTable.containsKey(argHash))
    {
      return mMethodTable.get(argHash);
    }
    else
    {
      Method meth = null;
      
      try
      {
        meth = mObj.getClass().getMethod(mMethodName, argTypes);
      }
      catch (Exception e)
      {
        println(delegateSignatureToString(argTypes) + " not found");
      }
      
      mMethodTable.put(argHash, meth);
      return meth;
    }
  }
  
  private Class[] objectTypes(Object[] objects)
  {
    Class [] types = new Class[objects.length];
    for (int idx = 0; idx < objects.length; idx++)
    {
      types[idx] = typeUnwrapPrimitive(objects[idx].getClass());
    }
    
    return types;
  }
  
  private String delegateSignatureToString(Class[] argTypes)
  {
    String s = mObj.toString() + "." + mMethodName + "(" + typesToString(argTypes) + ")";
    return s;
  }   
 
  private String typesToString(Class[] argTypes)
  {
    String argv = "";
    for (Class t: argTypes)
    {
      argv = argv + t.toString() + ",";
    }
    
    return argv;
  }

  private Map<String, Method> mMethodTable;
  private Object mObj;
  private String mMethodName;
}

/**
 */
public class DelegateList
{
  public DelegateList()
  {
    mListeners = new ArrayList<Delegate>();
  }
  
  public void invoke()
  {
    Iterator<Delegate> it = mListeners.iterator();
    while (it.hasNext())
    {
      it.next().invoke();
    }
  }
  
  public void invoke(Object arg)
  {
    Iterator<Delegate> it = mListeners.iterator();
    while (it.hasNext())
    {
      it.next().invoke(arg);
    }
  }
  
  public void addListener(Object obj, String methodName)
  {
    Delegate d = new Delegate(obj, methodName);
    
    if (findListener(d) < 0)
    {
      mListeners.add(d);
    }
  }
  
  public void removeListener(Object obj, String methodName)
  {
    int listenerIdx = findListener(new Delegate(obj, methodName));
    if (listenerIdx >= 0)
    {
      mListeners.remove(listenerIdx);
    }
  }
  
  private int findListener(Delegate d)
  {
    for (int idx = 0; idx < mListeners.size(); idx++)
    {
      if (mListeners.get(idx).equals(d))
      {
        return idx;
      }
    }
    
    return -1;
  }
  
  private List<Delegate> mListeners;
}

