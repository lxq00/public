//
//  Copyright (c)1998-2014, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: FuncTempl.h 3 2013-01-21 06:57:38Z  $
//

#define FUNCTION_TEMPL MACRO_JOIN(Function,FUNCTION_NUMBER)

/// \class FUNCTION_TEMPL
/// \brief ����ָ����ģ��
///
/// ֧����ͨ����ָ��ͳ�Ա����ָ��ı��棬�Ƚϣ����õȲ��������ڳ�Ա������
/// Ҫ����ֻ������ͨ������ǵ��̳е��࣬�����Ƕ�̳л���̳е��ࡣFUNCTION_TEMPL��һ���꣬
/// ���ݲ��������ᱻ�滻��Function���û�ͨ��FunctionN<R, T1, T2, T3,..,TN>��ʽ��ʹ�ã�
/// R��ʾ����ֵ���ͣ�TN��ʾ�������ͣ� N��ʾ��������������Ŀǰ������Ϊ6��ʾ�����£�
/// \code
/// int g(int x)
/// {
/// 	return x * 2;
/// }
/// class G
/// {
/// public:
/// 	int g(int x)
/// 	{
/// 		return x * 3;
/// 	}
/// }gg;
/// void test()
/// {
/// 	Function1<int, int> f1(g);
/// 	Function1<int, int> f2(&G::g, &gg);
/// 	assert(f1(1) = 2);
/// 	assert(f2(1) = 3);
/// 	f1 = f2;
/// 	assert(f1 == f2);
/// }
/// \endcode


////////////////////////////////////////////////////////////////////////////////

// Comma if nonzero number of arguments
#if FUNCTION_NUMBER == 0
#  define FUNCTION_COMMA
#else
#  define FUNCTION_COMMA ,
#endif // FUNCTION_NUMBER > 0


/////////////////////////////////////��������///////////////////////////////////////////

template <FUNCTION_CLASS_TYPES>
class FUNCTION_TEMPL
{
	typedef R(*PTR_FUNCTION)(FUNCTION_TYPES);
#ifdef GCCSUPORTC11
	typedef std::function<FUNCTION_STDFUNCTIONPTR> STD_FUNCTION;
#endif
	struct IDENT
	{
		void* func;
		void* obj;
		bool operator==(const IDENT& other) const
		{
			if (func == other.func && obj == other.obj)
			{
				return true;
			}
			return false;
		}
		bool operator<(const IDENT& other) const
		{
			if (func < other.func) return true;
			if (func == other.func && obj < other.obj) return true;
			return false;
		}
	};
	//function����������
	struct FUNCTION_OBJECT
	{
		FUNCTION_OBJECT() {}
		virtual ~FUNCTION_OBJECT() {}

		virtual bool empty() const = 0;

		virtual R invoke(FUNCTION_TYPE_ARGS) = 0;
		virtual shared_ptr<FUNCTION_OBJECT> clone() = 0;
		virtual IDENT ident() = 0;
#ifdef GCCSUPORTC11
		virtual STD_FUNCTION stdfunc() = 0;
#endif
	};

	//����ָ��Ķ���
	struct FUNCTION_POINTER :public FUNCTION_OBJECT
	{
		FUNCTION_POINTER(PTR_FUNCTION _f) :f(_f) {}

		virtual bool empty() const { return f == NULL; }

		virtual R invoke(FUNCTION_TYPE_ARGS)
		{
			if(f == NULL) return FUNCTION_RETURN<R>::getDefaultValue();

			return f(FUNCTION_ARGS);
		}
		virtual shared_ptr<FUNCTION_OBJECT> clone()
		{
			return shared_ptr<FUNCTION_POINTER>(new FUNCTION_POINTER(f));
		}
		virtual IDENT ident()
		{
			IDENT identi;
			identi.func = f;
			identi.obj = NULL;
			return identi;
		}
#ifdef GCCSUPORTC11
		virtual STD_FUNCTION stdfunc()
		{
			STD_FUNCTION stdf = f;
			return stdf;
		}
#endif

		PTR_FUNCTION f;
	};

	//��Ա�����ص��Ķ���
	template<typename O>
	struct FUNCTION_MEMBER :public FUNCTION_OBJECT
	{
		typedef R(O::*MEM_FUNCTION)(FUNCTION_TYPES);

		FUNCTION_MEMBER(MEM_FUNCTION _f, const O * _o):f(_f),o((O *)_o){}
		FUNCTION_MEMBER(MEM_FUNCTION _f, const shared_ptr<O>& _ptr) :f(_f), o(NULL), ptr(_ptr) {}


		virtual bool empty() const { return f == NULL; }

		virtual R invoke(FUNCTION_TYPE_ARGS)
		{
			O* optr = o;
			shared_ptr<O> ptrtmp = ptr.lock();
			if (ptrtmp != NULL) optr = ptrtmp.get();

			if (optr == NULL || f == NULL)
			{
				return FUNCTION_RETURN<R>::getDefaultValue();
			}

			return (optr->*f)(FUNCTION_ARGS);
		}
		virtual shared_ptr<FUNCTION_OBJECT> clone()
		{
			shared_ptr<FUNCTION_MEMBER> memptr(new FUNCTION_MEMBER(f,o));
			memptr.get()->ptr = ptr;

			return memptr;
		}
		virtual IDENT ident()
		{
			IDENT identi;
			identi.func = &f;
			identi.obj = o;
			shared_ptr<O> tmp = ptr.lock();
			if (tmp != NULL)
			{
				identi.obj = tmp.get();
			}
			return identi;
		}
#ifdef GCCSUPORTC11
		virtual STD_FUNCTION stdfunc()
		{
			STD_FUNCTION stdf;
			shared_ptr<O> ptrtmp = ptr.lock();
			if (ptrtmp != NULL)
			{
				stdf = std::bind(horrible_cast<MEM_FUNCTION>(f), ptrtmp FUNCTION_COMMA FUNCTION_STDPLACEHOLDERS);
			}
			else
			{
				stdf = std::bind(horrible_cast<MEM_FUNCTION>(f), o FUNCTION_COMMA FUNCTION_STDPLACEHOLDERS);
			}
			return stdf;
		}
#endif


		MEM_FUNCTION	f;
		O*				o;
		weak_ptr<O>		ptr;
	};

#ifdef GCCSUPORTC11
	struct FUNCTION_STDFUNCTION :public FUNCTION_OBJECT
	{
		FUNCTION_STDFUNCTION(const STD_FUNCTION& f) :stdf(f) {}
		virtual bool empty() const { return !stdf; }

		virtual R invoke(FUNCTION_TYPE_ARGS)
		{
			if (!stdf)
			{
				return FUNCTION_RETURN<R>::getDefaultValue();
			}

			return stdf(FUNCTION_ARGS);
		}
		virtual shared_ptr<FUNCTION_OBJECT> clone()
		{
			return shared_ptr<FUNCTION_STDFUNCTION>(new FUNCTION_STDFUNCTION(stdf));
		}
		virtual IDENT ident()
		{
			IDENT identi;
			identi.func = (void*)*(int*)&stdf;
			identi.obj = NULL;
			return identi;
		}
		virtual STD_FUNCTION stdfunc()
		{
			return stdf;
		}

		STD_FUNCTION stdf;
	};
#endif

	shared_ptr<FUNCTION_OBJECT> function;
public:
	/// ȱʡ���캯��
	FUNCTION_TEMPL( ){}

	/// ���ܳ�Ա����ָ�빹�캯��������ĳ�Ա������������ָ��󶨲����档
	/// \param [in] f ��ĳ�Ա����
	/// \param [in] o ������ָ��
	template<typename O>
	FUNCTION_TEMPL(R(O::*f)(FUNCTION_TYPES), const O * o)
	{
		bind(f,o);
	}

	template<typename O>
	FUNCTION_TEMPL(R(O::*f)(FUNCTION_TYPES), const shared_ptr<O>& ptr)
	{
		bind(f,ptr);
	}

	/// ������ͨ����ָ�빹�캯����������ͨ����ָ�롣
	/// \param [in] f ����ָ��
	FUNCTION_TEMPL(PTR_FUNCTION f)
	{
		bind(f);
	}
	FUNCTION_TEMPL(const FUNCTION_TEMPL& f)
	{
		swap(f);		
	}

#ifdef GCCSUPORTC11
	FUNCTION_TEMPL(const std::function<FUNCTION_STDFUNCTIONPTR>& f)
	{
		bind(f);
	}
#endif

	~FUNCTION_TEMPL()
	{
		realse();
	}
	/// �������캯��
	/// \param [in] f Դ����ָ�����
	FUNCTION_TEMPL& operator=(const FUNCTION_TEMPL& f)
	{
		swap(f);

		return *this;
	}
	/// ����ĳ�Ա������������ָ��󶨲����档�������͵ĺ���ָ�����=���������ʽת��ֱ����ɡ�
	template<typename O>
	void bind(R(O::*f)(FUNCTION_TYPES), const O * o)
	{
		realse();

		if(f == NULL || o == NULL)
		{
			return;
		}

		function = shared_ptr<FUNCTION_MEMBER<O> >(new FUNCTION_MEMBER<O>(f,o));
	}

	void bind(PTR_FUNCTION f)
	{
		realse();

		if (f == NULL)
		{
			return;
		}
		function = shared_ptr<FUNCTION_POINTER>(new FUNCTION_POINTER(f));
	}
	template<typename O>
	void bind(R(O::*f)(FUNCTION_TYPES), const shared_ptr<O>& ptr)
	{
		realse();

		if(f == NULL || ptr == NULL)
		{
			return;
		}
		function = shared_ptr<FUNCTION_MEMBER<O> >(new FUNCTION_MEMBER<O>(f, ptr));
	}
#ifdef GCCSUPORTC11
	void bind(const std::function<FUNCTION_STDFUNCTIONPTR>& f) 
	{
		realse();

		if (!f) return;

		function = shared_ptr<FUNCTION_STDFUNCTION>(new FUNCTION_STDFUNCTION(f));
	}
	operator std::function<FUNCTION_STDFUNCTIONPTR>()
	{
		return **this;
	}
	std::function<FUNCTION_STDFUNCTIONPTR> operator*()
	{
		std::function<FUNCTION_STDFUNCTIONPTR> f;

		shared_ptr<FUNCTION_OBJECT> tmp = function;
		if (tmp != NULL)
		{
			f = tmp->stdfunc();
		}

		return std::move(f);
	}
#endif
	operator bool() const
	{
		shared_ptr<FUNCTION_OBJECT> tmp = function;
		if (tmp == NULL) return false;

		return !tmp->empty();
	}

	bool operator!() const
	{
		return !(bool(*this));
	}

	/*bool operator==(const FUNCTION_TEMPL& tpl) const
	{
		shared_ptr<FUNCTION_OBJECT> tmp = function;
		shared_ptr<FUNCTION_OBJECT> tmp1 = tpl.function;
		if (tmp == NULL || tmp1 == NULL)
		{
			return false;
		}
		return tmp->ident() == tmp1->ident();
	}*/

	bool operator<(const FUNCTION_TEMPL& tpl) const
	{
		shared_ptr<FUNCTION_OBJECT> tmp = function;
		shared_ptr<FUNCTION_OBJECT> tmp1 = tpl.function;
		if (tmp == NULL || tmp1 == NULL)
		{
			return false;
		}
		return tmp->ident() < tmp1->ident();
	}

	/// ����()������������Ժ����������ʽ�����ñ���ĺ���ָ�롣
	inline R operator()(FUNCTION_TYPE_ARGS)
	{
		shared_ptr<FUNCTION_OBJECT> tmp = function;

		if (tmp == NULL)
		{
			return FUNCTION_RETURN<R>::getDefaultValue();
		}

		return tmp->invoke(FUNCTION_ARGS);
	}
	
	void realse()
	{
		function = NULL;
	}
	void swap(const FUNCTION_TEMPL& f)
	{
		if (&f == this)
			return;

		realse();

		if (f.function != NULL)
		{
			function = f.function->clone();
		}
	}
};

#undef FUNCTION_TEMPL
#undef FUNCTION_COMMA

