/**
 * Enables containers like TArray to be used on any arbitrary memory, inspired by TMaybeMappedAllocator.
 * Does *NOT* free the underlying memory on destruction or when wrapping a new pointer!
 * Use with TWrappingArray!
 */
class TStaticArrayAllocator
{
public:
	using SizeType = int32;

	enum { NeedsElementType = false };

	enum { RequireRangeCheck = true };

	class ForAnyElementType
	{
	public:
		ForAnyElementType() : Data(nullptr)
		{
		}

		~ForAnyElementType()
		{
		}

		void From(void* InData)
		{
			Data = static_cast<FScriptContainerElement*>(InData);
		}

		/**
		 * Moves the state of another allocator into this one.
		 * Assumes that the allocator is currently empty, i.e. memory may be allocated but any existing elements have already been destructed (if necessary).
		 * @param Other - The allocator to move the state from.  This allocator should be left in a valid empty state.
		 */
		void MoveToEmpty(ForAnyElementType& Other)
		{
			checkSlow(this != &Other);
			Data = Other.Data;
			Other.Data = nullptr;
		}

		// FContainerAllocatorInterface
		FScriptContainerElement* GetAllocation() const
		{
			return Data;
		}

		void ResizeAllocation(SizeType PreviousNumElements, SizeType NumElements, SIZE_T NumBytesPerElement)
		{
			// You can't do this.
			UE_LOG(LogTemp, Warning, TEXT("TStaticArrayAllocator::ResizeAllocation: Invalid attempt to reallocate data for static array"));
		}

		SizeType CalculateSlackReserve(SizeType NumElements, SIZE_T NumBytesPerElement) const { return NumElements; }
		SizeType CalculateSlackShrink(SizeType NumElements, SizeType NumAllocatedElements, SIZE_T NumBytesPerElement) const { return NumElements; }
		SizeType CalculateSlackGrow(SizeType NumElements, SizeType NumAllocatedElements, SIZE_T NumBytesPerElement) const { return NumElements; }
		SIZE_T GetAllocatedSize(SizeType NumAllocatedElements, SIZE_T NumBytesPerElement) const { return NumAllocatedElements * NumBytesPerElement; }
		bool HasAllocation() const { return !!Data; }
		SizeType GetInitialCapacity() const { return 0; }

	private:
		FScriptContainerElement* Data;

		ForAnyElementType(const ForAnyElementType&);
		ForAnyElementType& operator=(const ForAnyElementType&);
	};

	template <typename ElementType>
	class ForElementType : public ForAnyElementType
	{
	public:
		ForElementType()
		{
		}

		ElementType* GetAllocation() const { return static_cast<ElementType*>(ForAnyElementType::GetAllocation()); }
	};
};

/**
 * Used on any arbitrary memory, inspired by TMaybeMappedArray.
 * Does *NOT* free the underlying memory on destruction or when wrapping a new pointer!
 * 
 * Usage:
 * 
 * TWrappingArray<FVector> wrappingArray(pSomeMemory, 1024, 0);
 * 
 * -OR-
 * 
 * TWrappingArray<FVector> wrappingArray;
 * wrappingArray.Wrap(pSomeMemory, 1024, 0);
 * 
 */
template <typename T>
class TMStaticArray : public TArray<T, TStaticArrayAllocator>
{
private:
	T* RawPointer;

public:
	TMStaticArray()
	{
	}

	TMStaticArray& operator=(TMStaticArray&&) = default;
	TMStaticArray& operator=(const TMStaticArray&) = default;

	TMStaticArray(TMStaticArray&& Other)
	{
		From(Other.RawPointer, Other.ArrayMax, Other.ArrayNum);
	}

	TMStaticArray(const TMStaticArray& Other)
	{
		From(Other.RawPointer, Other.ArrayMax, Other.ArrayNum);
	}

	TMStaticArray(T* Data, const int32 Max, const int32 Num = 0)
	{
		From(Data, Max, Num);
	}

	/** Map to an owned TArray that doesn't use the shared data */
	TArray<T> Collect()
	{
		TArray<T> Rtn;
		for (auto Offset = 0; Offset < this->ArrayNum; Offset++)
		{
			Rtn.Add(RawPointer[Offset]);
		}
		return Rtn;
	}

	void From(T* Data, const int32 Max, const int32 Num)
	{
		this->ArrayMax = Max;
		this->ArrayNum = Num;
		this->AllocatorInstance.From(Data);
		this->RawPointer = Data;
	}
};

template <>
struct TAllocatorTraits<TStaticArrayAllocator> : TAllocatorTraitsBase<TStaticArrayAllocator>
{
	enum { SupportsMove = true };
};

template <typename T>
struct TIsContiguousContainer<TMStaticArray<T>>
{
	static constexpr bool Value = TIsContiguousContainer<TArray<T, TStaticArrayAllocator>>::Value;
};
