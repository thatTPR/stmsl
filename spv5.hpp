
namespace spv {



enum SourceLanguage{
Unknown=0,
ESSL=1,
GLSL=2,
OpenCL_C=3,
OpenCL_CPP=4,
HLSL=5,
CPP_for_OpenCL=6,
SYCL=7,
HERO_C=8,
NZSL=9,
WGSL=10,
Slang=11,
Zig=12,
Rust=13
}
enum ExecutionModel{
Vertex=0,
TessellationControl=1,
TessellationEvaluation=2,
Geometry=3,
Fragment=4,
GLCompute=5,
Kernel=6,
TaskNV=5267,
MeshNV=5268,
RayGenerationKHR =5313,//RayGenerationNV
IntersectionKHR =5314,//IntersectionNV
AnyHitKHR =5315,//AnyHitNV
ClosestHitKHR =5316,//ClosestHitNV
MissKHR =5317,//MissNV
CallableKHR =5318,//CallableNV
TaskEXT=5364,
MeshEXT=5365
};
enum AddressingModel{
    Logical=0,
    Physical32=1,
    Physical64=2,
    PhysicalStorageBuffer64 /*(PhysicalStorageBuffer64EXT)*/=5348
}
enum MemoryModel{
    Simple=0,
    GLSL450=1,
    OpenCL=2,
    Vulkan (VulkanKHR)=3
}
enum ExecutionMode {

    Invocations=0,
    SpacingEqual=1,
    SpacingFractionalEven=2,
SpacingFractionalOdd=3,
VertexOrderCw=4,
VertexOrderCcw=5,
PixelCenterInteger=6,
OriginUpperLeft=7,
OriginLowerLeft=8,
EarlyFragmentTests=9,
PointMode=10,
Xfb=11,
DepthReplacing=12,
DepthGreater=14,
DepthLess=15,
DepthUnchanged=16,
LocalSize=17,
LocalSizeHint=18,
InputPoints=19,
InputLines=20,
InputLinesAdjacency=21,
Triangles=22,
InputTrianglesAdjacency=23,
Quads=24,
Isolines=25,
OutputVertices=26,
OutputPoints=27,
OutputLineStrip=28,
OutputTriangleStrip=29,
VecTypeHint=30,
2 represents a 32-bit integer value.=0,
5 represents a 32-bit IEEE 754 float value.=3,
Only valid with the Kernel Execution Model.=6,
ContractionOff=31,
Initializer=33,
Finalizer=34,
SubgroupSize=35,
SubgroupsPerWorkgroup=36,
SubgroupsPerWorkgroupId=37,
LocalSizeId=38,
LocalSizeHintId=39,
NonCoherentColorAttachmentReadEXT=4169,
NonCoherentDepthAttachmentReadEXT=4170,
NonCoherentStencilAttachmentReadEXT=4171,
SubgroupUniformControlFlowKHR=4421,
PostDepthCoverage=4446,
DenormPreserve=4459,
DenormFlushToZero=4460,
SignedZeroInfNanPreserve=4461,
RoundingModeRTE=4462,
RoundingModeRTZ=4463,
NonCoherentTileAttachmentReadQCOM=4489,
TileShadingRateQCOM=4490,
EarlyAndLateFragmentTestsAMD=5017,
StencilRefReplacingEXT=5027,
CoalescingAMDX=5069,
IsApiEntryAMDX=5070,
MaxNodeRecursionAMDX=5071,
StaticNumWorkgroupsAMDX=5072,
ShaderIndexAMDX=5073,
MaxNumWorkgroupsAMDX=5077,
StencilRefUnchangedFrontAMD=5079,
StencilRefGreaterFrontAMD=5080,
StencilRefLessFrontAMD=5081,
StencilRefUnchangedBackAMD=5082,
StencilRefGreaterBackAMD=5083,
StencilRefLessBackAMD=5084,
QuadDerivativesKHR=5088,
RequireFullQuadsKHR=5089,
SharesInputWithAMDX=5102,
OutputLinesEXT =5269,//OutputLinesNV
OutputPrimitivesEXT =5270,//OutputPrimitivesNV
DerivativeGroupQuadsKHR =5289,//DerivativeGroupQuadsNV
DerivativeGroupLinearKHR =5290,//DerivativeGroupLinearNV
OutputTrianglesEXT =5298,//OutputTrianglesNV
PixelInterlockOrderedEXT=5366,
PixelInterlockUnorderedEXT=5367,
SampleInterlockOrderedEXT=5368,
SampleInterlockUnorderedEXT=5369,
ShadingRateInterlockOrderedEXT=5370,
ShadingRateInterlockUnorderedEXT=5371,
SharedLocalMemorySizeINTEL=5618,
RoundingModeRTPINTEL=5620,
RoundingModeRTNINTEL=5621,
FloatingPointModeALTINTEL=5622,
FloatingPointModeIEEEINTEL=5623,
MaxWorkgroupSizeINTEL=5893,
MaxWorkDimINTEL=5894,
NoGlobalOffsetINTEL=5895,
NumSIMDWorkitemsINTEL=5896,
SchedulerTargetFmaxMhzINTEL=5903,
MaximallyReconvergesKHR=6023,
FPFastMathDefault=6028,
StreamingInterfaceINTEL=6154,
RegisterMapInterfaceINTEL=6160,
NamedBarrierCountINTEL=6417,
MaximumRegistersINTEL=6461,
MaximumRegistersIdINTEL=6462,
NamedMaximumRegistersINTEL=6463}

enum StorageClass{
UniformConstant=0,
Input=1,
Uniform=2,
Output=3,
Workgroup=4,
CrossWorkgroup=5,
Private=6,
Function=7,
Generic=8,
PushConstant=9,
AtomicCounter=10,
Image=11,
StorageBuffer=12,
TileImageEXT=4172,
TileAttachmentQCOM=4491,
NodePayloadAMDX=5068,
CallableDataKHR =5328,//CallableDataNV
IncomingCallableDataKHR =5329,//IncomingCallableDataNV
RayPayloadKHR =5338,//RayPayloadNV
HitAttributeKHR =5339,//HitAttributeNV
IncomingRayPayloadKHR =5342,//IncomingRayPayloadNV
ShaderRecordBufferKHR =5343,//ShaderRecordBufferNV
PhysicalStorageBuffer =5349,//PhysicalStorageBufferEXT
HitObjectAttributeNV=5385,
TaskPayloadWorkgroupEXT=5402,
CodeSectionINTEL=5605,
DeviceOnlyINTEL=5936,
HostOnlyINTEL=5937}

enum DIm{
_1D=0,
_2D=1,
_3D=2,
Cube=3,
Rect=4,
Buffer=5,
SubpassData=6,
TileImageDataEXT=4173}
enum SamplerAddressingMode{
    None=0,
ClampToEdge=1,
Clamp=2,
Repeat=3,
RepeatMirrored=4}

enum SamplerFilterMode{
Nearest=0,
Linear=1
}
enum ImageFormat{
    
Unknown=0,
Rgba32f=1,
Rgba16f=2,
R32f=3,
Rgba8=4,
Rgba8Snorm=5,
Rg32f=6,
Rg16f=7,
R11fG11fB10f=8,
R16f=9,
Rgba16=10,
Rgb10A2=11,
Rg16=12,
Rg8=13,
R16=14,
R8=15,
Rgba16Snorm=16,
Rg16Snorm=17,
Rg8Snorm=18,
R16Snorm=19,
R8Snorm=20,
Rgba32i=21,
Rgba16i=22,
Rgba8i=23,
R32i=24,
Rg32i=25,
Rg16i=26,
Rg8i=27,
R16i=28,
R8i=29,
Rgba32ui=30,
Rgba16ui=31,
Rgba8ui=32,
R32ui=33,
Rgb10a2ui=34,
Rg32ui=35,
Rg16ui=36,
Rg8ui=37,
R16ui=38,
R8ui=39,
R64ui=40,
R64i=41}
enum  ImageChannelOrder {
    
R=0,
A=1,
RG=2,
RA=3,
RGB=4,
RGBA=5,
BGRA=6,
ARGB=7,
Intensity=8,
Luminance=9,
Rx=10,
RGx=11,
RGBx=12,
Depth=13,
DepthStencil=14,
sRGB=15,
sRGBx=16,
sRGBA=17,
sBGRA=18,
ABGR=19
}
enum ImageChannelDataType {


SnormInt8=0,
SnormInt16=1,
UnormInt8=2,
UnormInt16=3,
UnormShort565=4,
UnormShort555=5,
UnormInt101010=6,
SignedInt8=7,
SignedInt16=8,
SignedInt32=9,
UnsignedInt8=10,
UnsignedInt16=11,
UnsignedInt32=12,
HalfFloat=13,
Float=14,
UnormInt24=15,
UnormInt101010_2=16,
UnormInt10X6EXT=17,
UnsignedIntRaw10EXT=19,
UnsignedIntRaw12EXT=20,
UnormInt2_101010EXT=21,
UnsignedInt10X6EXT=22,
UnsignedInt12X4EXT=23,
UnsignedInt14X2EXT=24,
UnormInt12X4EXT=25,
UnormInt14X2EXT=26}
enum ImageOperands {


None=0,
Bias=1,
Lod=2,
Grad=4,
ConstOffset=8,
Offset=10,
ConstOffsets=20,
Sample=40,
MinLod=80,
MakeTexelAvailable =100,//MakeTexelAvailableKHR
MakeTexelVisible =200,//MakeTexelVisibleKHR
NonPrivateTexel =400,//NonPrivateTexelKHR
VolatileTexel =800,//VolatileTexelKHR
SignExtend=1000,
ZeroExtend=2000,
Nontemporal=4000,
Offsets=10000
}
enum FPFastMathMode{


None=0x0,
NotNaN=0x1,
NotInf=0x2,
NSZ=0x4,
AllowRecip=0x8,
Fast=0x10,
AllowContract =0x10000,/
AllowReassoc =0x20000,/
AllowTransform=0x40000
}
enum FPRoundingMode {

    RTE=0,
    RTZ=1,
    RTP=2,
    RTN=3
}
enum  LinkageType{
    Export=0,
    Import=1,
    LinkOnceODR=2}
enum AccessQualifier {

    ReadOnly=0,
    WriteOnly=1,
    ReadWrite=2
}
enum  FunctionParameterAttribute{
    Zext=0,
    Sext=1,
    ByVal=2,
    Sret=3,
    NoAlias=4,
    NoCapture=5,
    NoWrite=6,
    NoReadWrite=7,
    RuntimeAlignedINTEL=5940
}
enum Decoration{
RelaxedPrecision=0,
SpecId=1,
Block=2,
BufferBlock=3,
RowMajor=4,
ColMajor=5,
ArrayStride=6,
MatrixStride=7,
GLSLShared=8,
GLSLPacked=9,
CPacked=10,
BuiltIn=11,
NoPerspective=13,
Flat=14,
Patch=15,
Centroid=16,
Sample=17,
Invariant=18,
Restrict=19,
Aliased=20,
Volatile=21,
Constant=22,
Coherent=23,
NonWritable=24,
NonReadable=25,
Uniform=26,
UniformId=27,
SaturatedConversion=28,
Stream=29,
Location=30,
Component=31,
Index=32,
Binding=33,
DescriptorSet=34,
Offset=35,
XfbBuffer=36,
XfbStride=37,
FuncParamAttr=38,
FPRoundingMode=39,
FPFastMathMode=40,
LinkageAttributes=41,
NoContraction=42,
InputAttachmentIndex=43,
Alignment=44,
MaxByteOffset=45,
AlignmentId=46,
MaxByteOffsetId=47,
SaturatedToLargestFloat8NormalConversionEXT=4216,
NoSignedWrap=4469,
NoUnsignedWrap=4470,
WeightTextureQCOM=4487,
BlockMatchTextureQCOM=4488,
BlockMatchSamplerQCOM=4499,
ExplicitInterpAMD=4999,
NodeSharesPayloadLimitsWithAMDX=5019,
NodeMaxPayloadsAMDX=5020,
TrackFinishWritingAMDX=5078,
PayloadNodeNameAMDX=5091,
PayloadNodeBaseIndexAMDX=5098,
PayloadNodeSparseArrayAMDX=5099,
PayloadNodeArraySizeAMDX=5100,
PayloadDispatchIndirectAMDX=5105,
OverrideCoverageNV=5248,
PassthroughNV=5250,
ViewportRelativeNV=5252,
SecondaryViewportRelativeNV=5256,
PerPrimitiveEXT =5271,//PerPrimitiveNV
PerViewNV=5272,
PerTaskNV=5273,
PerVertexKHR =5285,//PerVertexNV
NonUniform =5300,//NonUniformEXT
RestrictPointer =5355,//RestrictPointerEXT
AliasedPointer =5356,//AliasedPointerEXT
HitObjectShaderRecordBufferNV=5386,
BindlessSamplerNV=5398,
BindlessImageNV=5399,
BoundSamplerNV=5400,
BoundImageNV=5401,
SIMTCallINTEL=5599,
ReferencedIndirectlyINTEL=5602,
ClobberINTEL=5607,
SideEffectsINTEL=5608,
VectorComputeVariableINTEL=5624,
FuncParamIOKindINTEL=5625,
VectorComputeFunctionINTEL=5626,
StackCallINTEL=5627,
GlobalVariableOffsetINTEL=5628,
CounterBuffer =5634,//HlslCounterBufferGOOGLE
UserSemantic =5635,//HlslSemanticGOOGLE
UserTypeGOOGLE=5636,
FunctionRoundingModeINTEL=5822,
FunctionDenormModeINTEL=5823,
RegisterINTEL=5825,
MemoryINTEL=5826,
NumbanksINTEL=5827,
BankwidthINTEL=5828,
MaxPrivateCopiesINTEL=5829,
SinglepumpINTEL=5830,
DoublepumpINTEL=5831,
MaxReplicatesINTEL=5832,
SimpleDualPortINTEL=5833,
MergeINTEL=5834,
BankBitsINTEL=5835,
ForcePow2DepthINTEL=5836,
StridesizeINTEL=5883,
WordsizeINTEL=5884,
TrueDualPortINTEL=5885,
BurstCoalesceINTEL=5899,
CacheSizeINTEL=5900,
DontStaticallyCoalesceINTEL=5901,
PrefetchINTEL=5902,
StallEnableINTEL=5905,
FuseLoopsInFunctionINTEL=5907,
MathOpDSPModeINTEL=5909,
AliasScopeINTEL=5914,
NoAliasINTEL=5915,
InitiationIntervalINTEL=5917,
MaxConcurrencyINTEL=5918,
PipelineEnableINTEL=5919,
BufferLocationINTEL=5921,
IOPipeStorageINTEL=5944,
FunctionFloatingPointModeINTEL=6080,
SingleElementVectorINTEL=6085,
VectorComputeCallableFunctionINTEL=6087,
MediaBlockIOINTEL=6140,
StallFreeINTEL=6151,
FPMaxErrorDecorationINTEL=6170,
LatencyControlLabelINTEL=6172,
LatencyControlConstraintINTEL=6173,
ConduitKernelArgumentINTEL=6175,
RegisterMapKernelArgumentINTEL=6176,
MMHostInterfaceAddressWidthINTEL=6177,
MMHostInterfaceDataWidthINTEL=6178,
MMHostInterfaceLatencyINTEL=6179,
MMHostInterfaceReadWriteModeINTEL=6180,
MMHostInterfaceMaxBurstINTEL=6181,
MMHostInterfaceWaitRequestINTEL=6182,
StableKernelArgumentINTEL=6183,
HostAccessINTEL=6188,
InitModeINTEL=6190,
ImplementInRegisterMapINTEL=6191,
ConditionalINTEL=6247,
CacheControlLoadINTEL=6442,
CacheControlStoreINTEL=6443
}
enum Builtin {


Position=0,
PointSize=1,
ClipDistance=3,
CullDistance=4,
VertexId=5,
InstanceId=6,
PrimitiveId=7,
InvocationId=8,
Layer=9,
ViewportIndex=10,
TessLevelOuter=11,
TessLevelInner=12,
TessCoord=13,
PatchVertices=14,
FragCoord=15,
PointCoord=16,
FrontFacing=17,
SampleId=18,
SamplePosition=19,
SampleMask=20,
FragDepth=22,
HelperInvocation=23,
NumWorkgroups=24,
WorkgroupSize=25,
WorkgroupId=26,
LocalInvocationId=27,
GlobalInvocationId=28,
LocalInvocationIndex=29,
WorkDim=30,
GlobalSize=31,
EnqueuedWorkgroupSize=32,
GlobalOffset=33,
GlobalLinearId=34,
SubgroupSize=36,
SubgroupMaxSize=37,
NumSubgroups=38,
NumEnqueuedSubgroups=39,
SubgroupId=40,
SubgroupLocalInvocationId=41,
VertexIndex=42,
InstanceIndex=43,
CoreIDARM=4160,
CoreCountARM=4161,
CoreMaxIDARM=4162,
WarpIDARM=4163,
WarpMaxIDARM=4164,
SubgroupEqMask =4416,//SubgroupEqMaskKHR
SubgroupGeMask =4417,//SubgroupGeMaskKHR
SubgroupGtMask =4418,//SubgroupGtMaskKHR
SubgroupLeMask =4419,//SubgroupLeMaskKHR
SubgroupLtMask =4420,//SubgroupLtMaskKHR
BaseVertex=4424,
BaseInstance=4425,
DrawIndex=4426,
PrimitiveShadingRateKHR=4432,
DeviceIndex=4438,
ViewIndex=4440,
ShadingRateKHR=4444,
TileOffsetQCOM=4492,
TileDimensionQCOM=4493,
TileApronSizeQCOM=4494,
BaryCoordNoPerspAMD=4992,
BaryCoordNoPerspCentroidAMD=4993,
BaryCoordNoPerspSampleAMD=4994,
BaryCoordSmoothAMD=4995,
BaryCoordSmoothCentroidAMD=4996,
BaryCoordSmoothSampleAMD=4997,
BaryCoordPullModelAMD=4998,
FragStencilRefEXT=5014,
RemainingRecursionLevelsAMDX=5021,
ShaderIndexAMDX=5073,
ViewportMaskNV=5253,
SecondaryPositionNV=5257,
SecondaryViewportMaskNV=5258,
PositionPerViewNV=5261,
ViewportMaskPerViewNV=5262,
FullyCoveredEXT=5264,
TaskCountNV=5274,
PrimitiveCountNV=5275,
PrimitiveIndicesNV=5276,
ClipDistancePerViewNV=5277,
CullDistancePerViewNV=5278,
LayerPerViewNV=5279,
MeshViewCountNV=5280,
MeshViewIndicesNV=5281,
BaryCoordKHR =5286,//BaryCoordNV
BaryCoordNoPerspKHR =5287,//BaryCoordNoPerspNV
FragSizeEXT =5292,//FragmentSizeNV
FragInvocationCountEXT =5293,//InvocationsPerPixelNV
PrimitivePointIndicesEXT=5294,
PrimitiveLineIndicesEXT=5295,
PrimitiveTriangleIndicesEXT=5296,
CullPrimitiveEXT=5299,
LaunchIdKHR =5319,//LaunchIdNV
LaunchSizeKHR =5320,//LaunchSizeNV
WorldRayOriginKHR =5321,//WorldRayOriginNV
WorldRayDirectionKHR =5322,//WorldRayDirectionNV
ObjectRayOriginKHR =5323,//ObjectRayOriginNV
ObjectRayDirectionKHR =5324,//ObjectRayDirectionNV
RayTminKHR =5325,//RayTminNV
RayTmaxKHR =5326,//RayTmaxNV
InstanceCustomIndexKHR =5327,//InstanceCustomIndexNV
ObjectToWorldKHR =5330,//ObjectToWorldNV
WorldToObjectKHR =5331,//WorldToObjectNV
HitTNV=5332,
HitKindKHR =5333,//HitKindNV
CurrentRayTimeNV=5334,
HitTriangleVertexPositionsKHR=5335,
HitMicroTriangleVertexPositionsNV=5337,
HitMicroTriangleVertexBarycentricsNV=5344,
IncomingRayFlagsKHR =5351,//IncomingRayFlagsNV
RayGeometryIndexKHR=5352,
HitIsSphereNV=5359,
HitIsLSSNV=5360,
HitSpherePositionNV=5361,
WarpsPerSMNV=5374,
SMCountNV=5375,
WarpIDNV=5376,
SMIDNV=5377,
HitLSSPositionsNV=5396,
HitKindFrontFacingMicroTriangleNV=5405,
HitKindBackFacingMicroTriangleNV=5406,
HitSphereRadiusNV=5420,
HitLSSRadiiNV=5421,
ClusterIDNV=5436,
CullMaskKHR=6021
}
enum SelectionControl{
    None=0x0,
    Flatten=0x1,
    DontFlatten=0x2
}
enum LoopControl{
    None=0x0,
    Unroll=0x1,
    DontUnroll=0x2,
    DependencyInfinite=0x4,
    DependencyLength=0x8,
    MinIterations=0x10,
    MaxIterations=0x20,
    IterationMultiple=0x40,
    PeelCount=0x80,
    PartialCount=0x100,
    InitiationIntervalINTEL=0x10000,
    MaxConcurrencyINTEL=0x20000,
    DependencyArrayINTEL=0x40000,
    PipelineEnableINTEL=0x80000,
    LoopCoalesceINTEL=0x100000,
    MaxInterleavingINTEL=0x200000,
    SpeculatedIterationsINTEL=0x400000,
    NoFusionINTEL=0x800000,
    LoopCountINTEL=0x1000000,
    MaxReinvocationDelayINTEL=0x2000000
}
enum FunctionControl {
    None=0x0,
    Inline=0x1,
    DontInline=0x2,
    Pure=0x4,
    Const=0x8,
    OptNoneEXT=0x10000,// (OptNoneINTEL)
}
enum MemorySemantics_id{
    None=0x0,//(Relaxed)
    Acquire=0x2,
    Release=0x4,
    AcquireRelease=0x8,
    SequentiallyConsistent=0x10,
    UniformMemory=0x40,
    SubgroupMemory=0x80,
    WorkgroupMemory=0x100,
    CrossWorkgroupMemory=0x200,
    AtomicCounterMemory=0x400,
    ImageMemory=0x800,
    OutputMemory=0x1000,//(OutputMemoryKHR)
    MakeAvailable=0x2000,//(MakeAvailableKHR)
    MakeVisible=0x4000,//(MakeVisibleKHR)
    Volatile=0x8000
}
enum  MemoryOperands{
    None=0x0,
    Volatile=0x1,
    Aligned=0x2,
    Nontemporal=0x4,
    MakePointerAvailable =0x8,//(MakePointerAvailableKHR)
    MakePointerVisible =0x10,//(MakePointerVisibleKHR)
    NonPrivatePointer =0x20,//(NonPrivatePointerKHR)
    AliasScopeINTELMask=0x10000,
    NoAliasINTELMask=0x20000
}
enum Scope_id{
    CrossDevice=0,
    Device=1,
    Workgroup=2,
    Subgroup=3,
    Invocation=4,
    QueueFamily =5,//(QueueFamilyKHR)
    ShaderCallKHR=6
}
enum GroupOperation{
    Reduce=0,
    InclusiveScan=1,
    ExclusiveScan=2,
    ClusteredReduce=3,
    PartitionedReduceNV=6,
    PartitionedInclusiveScanNV=7,
    PartitionedExclusiveScanNV=8
}
enum KernelEnqueueFlags{
    NoWait=0,
    WaitKernel=1,
    WaitWorkGroup=2
}
enum KernelProfilingInfo {
    None=0,
    CmdExecTime=1
};
enum Capability{

Matrix=0,
Shader=1,
Geometry=2,
Tessellation=3,
Addresses=4,
Linkage=5,
Kernel=6,
Vector16=7,
Float16Buffer=8,
Float16=9,
Float64=10,
Int64=11,
Int64Atomics=12,
ImageBasic=13,
ImageReadWrite=14,
ImageMipmap=15,
Pipes=17,
Groups=18,
DeviceEnqueue=19,
LiteralSampler=20,
AtomicStorage=21,
Int16=22,
TessellationPointSize=23,
GeometryPointSize=24,
ImageGatherExtended=25,
StorageImageMultisample=27,
UniformBufferArrayDynamicIndexing=28,
SampledImageArrayDynamicIndexing=29,
StorageBufferArrayDynamicIndexing=30,
StorageImageArrayDynamicIndexing=31,
ClipDistance=32,
CullDistance=33,
ImageCubeArray=34,
SampleRateShading=35,
ImageRect=36,
SampledRect=37,
GenericPointer=38,
Int8=39,
InputAttachment=40,
SparseResidency=41,
MinLod=42,
Sampled1D=43,
Image1D=44,
SampledCubeArray=45,
SampledBuffer=46,
ImageBuffer=47,
ImageMSArray=48,
StorageImageExtendedFormats=49,
ImageQuery=50,
DerivativeControl=51,
InterpolationFunction=52,
TransformFeedback=53,
GeometryStreams=54,
StorageImageReadWithoutFormat=55,
StorageImageWriteWithoutFormat=56,
MultiViewport=57,
SubgroupDispatch=58,
NamedBarrier=59,
PipeStorage=60,
GroupNonUniform=61,
GroupNonUniformVote=62,
GroupNonUniformArithmetic=63,
GroupNonUniformBallot=64,
GroupNonUniformShuffle=65,
GroupNonUniformShuffleRelative=66,
GroupNonUniformClustered=67,
GroupNonUniformQuad=68,
ShaderLayer=69,
ShaderViewportIndex=70,
UniformDecoration=71,
CoreBuiltinsARM=4165,
TileImageColorReadAccessEXT=4166,
TileImageDepthReadAccessEXT=4167,
TileImageStencilReadAccessEXT=4168,
TensorsARM=4174,
StorageTensorArrayDynamicIndexingARM=4175,
StorageTensorArrayNonUniformIndexingARM=4176,
GraphARM=4191,
CooperativeMatrixLayoutsARM=4201,
Float8EXT=4212,
Float8CooperativeMatrixEXT=4213,
FragmentShadingRateKHR=4422,
SubgroupBallotKHR=4423,
DrawParameters=4427,
WorkgroupMemoryExplicitLayoutKHR=4428,
WorkgroupMemoryExplicitLayout8BitAccessKHR=4429,
WorkgroupMemoryExplicitLayout16BitAccessKHR=4430,
SubgroupVoteKHR=4431,
StorageBuffer16BitAccess =4433,//StorageUniformBufferBlock16
UniformAndStorageBuffer16BitAccess =4434,//StorageUniform16
StoragePushConstant16=4435,
StorageInputOutput16=4436,
DeviceGroup=4437,
MultiView=4439,
VariablePointersStorageBuffer=4441,
VariablePointers=4442,
AtomicStorageOps=4445,
SampleMaskPostDepthCoverage=4447,
StorageBuffer8BitAccess=4448,
UniformAndStorageBuffer8BitAccess=4449,
StoragePushConstant8=4450,
DenormPreserve=4464,
DenormFlushToZero=4465,
SignedZeroInfNanPreserve=4466,
RoundingModeRTE=4467,
RoundingModeRTZ=4468,
RayQueryProvisionalKHR=4471,
RayQueryKHR=4472,
UntypedPointersKHR=4473,
RayTraversalPrimitiveCullingKHR=4478,
RayTracingKHR=4479,
TextureSampleWeightedQCOM=4484,
TextureBoxFilterQCOM=4485,
TextureBlockMatchQCOM=4486,
TileShadingQCOM=4495,
CooperativeMatrixConversionQCOM=4496,
TextureBlockMatch2QCOM=4498,
Float16ImageAMD=5008,
ImageGatherBiasLodAMD=5009,
FragmentMaskAMD=5010,
StencilExportEXT=5013,
ImageReadWriteLodAMD=5015,
Int64ImageEXT=5016,
ShaderClockKHR=5055,
ShaderEnqueueAMDX=5067,
QuadControlKHR=5087,
Int4TypeINTEL=5112,
Int4CooperativeMatrixINTEL=5114,
BFloat16TypeKHR=5116,
BFloat16DotProductKHR=5117,
BFloat16CooperativeMatrixKHR=5118,
SampleMaskOverrideCoverageNV=5249,
GeometryShaderPassthroughNV=5251,
ShaderViewportIndexLayerEXT =5254,//(ShaderViewportIndexLayerNV)
ShaderViewportMaskNV=5255,
ShaderStereoViewNV=5259,
PerViewAttributesNV=5260,
FragmentFullyCoveredEXT=5265,
MeshShadingNV=5266,
ImageFootprintNV=5282,
MeshShadingEXT=5283,
FragmentBarycentricKHR =5284,//(FragmentBarycentricNV)
ComputeDerivativeGroupQuadsKHR =5288,//(ComputeDerivativeGroupQuadsNV)
FragmentDensityEXT =5291,//(ShadingRateNV)
GroupNonUniformPartitionedNV=5297,
ShaderNonUniform=5301,// (ShaderNonUniformEXT)
RuntimeDescriptorArray=5302,// (RuntimeDescriptorArrayEXT)
InputAttachmentArrayDynamicIndexing=5303,// (InputAttachmentArrayDynamicIndexingEXT)
UniformTexelBufferArrayDynamicIndexing=5304,// (UniformTexelBufferArrayDynamicIndexingEXT)
StorageTexelBufferArrayDynamicIndexing=5305,// (StorageTexelBufferArrayDynamicIndexingEXT)
UniformBufferArrayNonUniformIndexing=5306,// (UniformBufferArrayNonUniformIndexingEXT)
SampledImageArrayNonUniformIndexing=5307,// (SampledImageArrayNonUniformIndexingEXT)
StorageBufferArrayNonUniformIndexing=5308,// (StorageBufferArrayNonUniformIndexingEXT)
StorageImageArrayNonUniformIndexing=5309,// (StorageImageArrayNonUniformIndexingEXT)
InputAttachmentArrayNonUniformIndexing=5310,// (InputAttachmentArrayNonUniformIndexingEXT)
UniformTexelBufferArrayNonUniformIndexing=5311,// (UniformTexelBufferArrayNonUniformIndexingEXT)
StorageTexelBufferArrayNonUniformIndexing=5312,// (StorageTexelBufferArrayNonUniformIndexingEXT)
RayTracingPositionFetchKHR=5336,
RayTracingNV=5340,
RayTracingMotionBlurNV=5341,
VulkanMemoryModel =5345,//VulkanMemoryModelKHR
VulkanMemoryModelDeviceScope =5346,//VulkanMemoryModelDeviceScopeKHR
PhysicalStorageBufferAddresses =5347,//PhysicalStorageBufferAddressesEXT
ComputeDerivativeGroupLinearKHR =5350,//ComputeDerivativeGroupLinearNV
RayTracingProvisionalKHR=5353,
CooperativeMatrixNV=5357,
FragmentShaderSampleInterlockEXT=5363,
FragmentShaderShadingRateInterlockEXT=5372,
ShaderSMBuiltinsNV=5373,
FragmentShaderPixelInterlockEXT=5378,
DemoteToHelperInvocation =5379,//DemoteToHelperInvocationEXT
DisplacementMicromapNV=5380,
RayTracingOpacityMicromapEXT=5381,
ShaderInvocationReorderNV=5383,
BindlessTextureNV=5390,
RayQueryPositionFetchKHR=5391,
CooperativeVectorNV=5394,
AtomicFloat16VectorNV=5404,
RayTracingDisplacementMicromapNV=5409,
RawAccessChainsNV=5414,
RayTracingSpheresGeometryNV=5418,
RayTracingLinearSweptSpheresGeometryNV=5419,
CooperativeMatrixReductionsNV=5430,
CooperativeMatrixConversionsNV=5431,
CooperativeMatrixPerElementOperationsNV=5432,
CooperativeMatrixTensorAddressingNV=5433,
CooperativeMatrixBlockLoadsNV=5434,
CooperativeVectorTrainingNV=5435,
RayTracingClusterAccelerationStructureNV=5437,
TensorAddressingNV=5439,
SubgroupShuffleINTEL=5568,
SubgroupBufferBlockIOINTEL=5569,
SubgroupImageBlockIOINTEL=5570,
SubgroupImageMediaBlockIOINTEL=5579,
RoundToInfinityINTEL=5582,
FloatingPointModeINTEL=5583,
IntegerFunctions2INTEL=5584,
FunctionPointersINTEL=5603,
IndirectReferencesINTEL=5604,
AsmINTEL=5606,
AtomicFloat32MinMaxEXT=5612,
AtomicFloat64MinMaxEXT=5613,
AtomicFloat16MinMaxEXT=5616,
VectorComputeINTEL=5617,
VectorAnyINTEL=5619,
ExpectAssumeKHR=5629,
SubgroupAvcMotionEstimationINTEL=5696,
SubgroupAvcMotionEstimationIntraINTEL=5697,
SubgroupAvcMotionEstimationChromaINTEL=5698,
VariableLengthArrayINTEL=5817,
FunctionFloatControlINTEL=5821,
FPGAMemoryAttributesINTEL=5824,
FPFastMathModeINTEL=5837,
ArbitraryPrecisionIntegersINTEL=5844,
ArbitraryPrecisionFloatingPointINTEL=5845,
UnstructuredLoopControlsINTEL=5886,
FPGALoopControlsINTEL=5888,
KernelAttributesINTEL=5892,
FPGAKernelAttributesINTEL=5897,
FPGAMemoryAccessesINTEL=5898,
FPGAClusterAttributesINTEL=5904,
LoopFuseINTEL=5906,
FPGADSPControlINTEL=5908,
MemoryAccessAliasingINTEL=5910,
FPGAInvocationPipeliningAttributesINTEL=5916,
FPGABufferLocationINTEL=5920,
ArbitraryPrecisionFixedPointINTEL=5922,
USMStorageClassesINTEL=5935,
RuntimeAlignedAttributeINTEL=5939,
IOPipesINTEL=5943,
BlockingPipesINTEL=5945,
FPGARegINTEL=5948,
DotProductInputAll =6016,//DotProductInputAllKHR
DotProductInput4x8Bit =6017,//DotProductInput4x8BitKHR
DotProductInput4x8BitPacked =6018,//DotProductInput4x8BitPackedKHR
DotProduct =6019,//DotProductKHR
RayCullMaskKHR=6020,
CooperativeMatrixKHR=6022,
ReplicatedCompositesEXT=6024,
BitInstructions=6025,
GroupNonUniformRotateKHR=6026,
FloatControls2=6029,
AtomicFloat32AddEXT=6033,
AtomicFloat64AddEXT=6034,
LongCompositesINTEL=6089,
OptNoneEXT =6094,//OptNoneINTEL
AtomicFloat16AddEXT=6095,
DebugInfoModuleINTEL=6114,
BFloat16ConversionINTEL=6115,
SplitBarrierINTEL=6141,
ArithmeticFenceEXT=6144,
FPGAClusterAttributesV2INTEL=6150,
FPGAKernelAttributesv2INTEL=6161,
TaskSequenceINTEL=6162,
FPMaxErrorINTEL=6169,
FPGALatencyControlINTEL=6171,
FPGAArgumentInterfacesINTEL=6174,
GlobalVariableHostAccessINTEL=6187,
GlobalVariableFPGADecorationsINTEL=6189,
SubgroupBufferPrefetchINTEL=6220,
Subgroup2DBlockIOINTEL=6228,
Subgroup2DBlockTransformINTEL=6229,
Subgroup2DBlockTransposeINTEL=6230,
SubgroupMatrixMultiplyAccumulateINTEL=6236,
TernaryBitwiseFunctionINTEL=6241,
SpecConditionalINTEL=6245,
FunctionVariantsINTEL=6246,
GroupUniformArithmeticKHR=6400,
TensorFloat32RoundingINTEL=6425,
MaskedGatherScatterINTEL=6427,
CacheControlsINTEL=6441,
RegisterLimitsINTEL=6460,
BindlessImagesINTEL=6528}
enum RayFlags{
None=0,
OpaqueKHR=1,
NoOpaqueKHR=2,
TerminateOnFirstHitKHR=4,
SkipClosestHitShaderKHR=8,
CullBackFacingTrianglesKHR=10,
CullFrontFacingTrianglesKHR=20,
CullOpaqueKHR=40,
CullNoOpaqueKHR=80,
SkipTrianglesKHR =100,//SkipBuiltinPrimitivesNV
SkipAABBsKHR=200,
ForceOpacityMicromap2StateEXT=400}
enum RayQueryIntersection {
RayQueryCandidateIntersectionKHR=0,
RayQueryCommittedIntersectionKHR=1}
enum RayQueryCommittedType {
    RayQueryCommittedIntersectionNoneKHR=0,
    RayQueryCommittedIntersectionTriangleKHR=1,
    RayQueryCommittedIntersectionGeneratedKHR=2
}
enum RayQueryCandidateType{
    RayQueryCandidateIntersectionTriangleKHR=0,
    RayQueryCandidateIntersectionAABBKHR=1
}
enum FragmentShadingRate {
    None=0x0,
    Vertical2Pixels=0x1,
    Vertical4Pixels=0x2,
    Horizontal2Pixels=0x4,
    Horizontal4Pixels=0x8
};
enum FPDenormMode {
    Preserve=0,
    FlushToZero=1
}
enum FPOperationMode {
    IEEE=0,
    ALT=1
};
enum QuantizationMode{
    TRN=0,
    TRN_ZERO=1,
    RND=2,
    RND_ZERO=3,
    RND_INF=4,
    RND_MIN_INF=5,
    RND_CONV=6,
    RND_CONV_ODD=7
}
enum OverflowMode{
    WRAP=0,
    SAT=1,
    SAT_ZERO=2,
    SAT_SYM=3
}
enum PackedVectorFormat {
    PackedVectorFormat4x8Bit=0// (PackedVectorFormat4x8BitKHR)
}
enum CooperativeMatrixOperands {

    None=0x0,
    MatrixASignedComponentsKHR=0x1,
    MatrixBSignedComponentsKHR=0x2,
    MatrixCSignedComponentsKHR=0x4,
    MatrixResultSignedComponentsKHR=0x8,
    SaturatingAccumulationKHR=0x10
}
enum CooperativeMatrixLayout {
    RowMajorKHR=1,
    ColumnMajorKHR=1,
    RowBlockedInterleavedARM=4202,
    ColumnBlockedInterleavedARM=4203
};
enum CooperativeMatrixUse {
    MatrixAKHR=1,    
    MatrixBKHR=1,
    MatrixAccumulatorKHR=2
}
enum CooperativeMatrixReduceMode{

    None=0,
    Row=1,
    Column=2,
    _2x2=4
}
enum TensorClampMode {
Undefined=0,
    Constant=1,
    ClampToEdge=2,
    Repeat=3,
    RepeatMirrored=4
}
enum TensorAddressingOperands {
    None=0,
    TensorView=1,
    DecodeFunc=2
}
enum TensorOperands {
    None=0,
    NontemporalARM=1,
    OutOfBoundsValueARM=2,
    MakeElementAvailableARM=4,
    MakeElementVisibleARM=8,
    NonPrivateElementARM=10
}
enum InitializationModeQualifier {
InitOnDeviceReprogramINTEL=0,
    InitOnDeviceResetINTEL=1
};
enum HostAccessQualifier {
    NoneINTEL=0,
    ReadINTEL=1,
    WriteINTEL=2,
    ReadWriteINTEL=3
}
enum LoadCacheControl {
UncachedINTEL=0,
    CachedINTEL=1,
    StreamingINTEL=2,
    InvalidateAfterReadINTEL=3,
    ConstCachedINTEL=4
};
enum StoreCacheControl {

    UncachedINTEL0=0,
    WriteThroughINTEL=1,
    WriteBackINTEL=2,
    StreamingINTEL=3
}
enum NamedMaximumNumberofRegister {
    AUTOINTEL=0
}
enum MatrixMultiplyAccumulateOperands {
    None=0,
    MatrixASignedComponentsINTEL=1,
    MatrixBSignedComponentsINTEL=2,
    MatrixCBFloat16INTEL=4,
    MatrixResultBFloat16INTEL=8,
    MatrixAPackedInt8INTEL=10,
    MatrixBPackedInt8INTEL=20,
    MatrixAPackedInt4INTEL=40,
    MatrixBPackedInt4INTEL=80,
    MatrixATF32INTEL=100,
    MatrixBTF32INTEL=200,
    MatrixAPackedFloat16INTEL=400,
    MatrixBPackedFloat16INTEL=800,
    MatrixAPackedBFloat16INTEL=1000,
    MatrixBPackedBFloat16INTEL=2000
}
enum RawAccessChainOperands{
 None=0x0,
RobustnessPerComponentNV=0x1,
RobustnessPerElementNV=0x2
}
enum FPEncoding {
  
    BFloat16KHR=0,
    Float8E4M3EXT=4214,
    Float8E5M2EXT=4215
}
enum CooperativeVectorMatrixLayout {
RowMajorNV=0,
ColumnMajorNV=1,
InferencingOptimalNV=2,
TrainingOptimalNV=3
};
enum CooperativeVectorMatrixComponentType{

    
    Float16NC=0,
    Float32NV=1,
    Float64NV=2,
    SignedInt8NV=3,
    SignedInt16NV=4,
    SignedInt32NV=5,
    SignedInt64NV=6,
    UnsignedInt8NV=7,
    UnsignedInt16NV=8,
    UnsignedInt32NV=9,
    UnsignedInt64NV=10,
    SignedInt8PackedNV=1000491000,
    UnsignedInt8PackedNV=1000491001,
    FloatE4M3NV=1000491002,
    FloatE5M2NV=1000491003
}

/// INSTRUCTIONS
enum MiscellaneousInstructions {
    OpNop=0,//1
    OpUndef=1,   //3
OpSizeOf=321,//4
OpCooperativeMatrixLengthKHR=4460,//4
OpAssumeTrueKHR=5630,//2
OpExpectKHR=5631,//5
OpArithmeticFenceEXT=6145,//4
};
enum DebugInstructions {
    OpSourceContinued=2,//=2+var,
    OpSource=3,//=3+var,
    OpSourceExtension=4,//=2+var,
    OpName=5,//=3+var,
OpMemberName=6,//=4+var,
OpString=7,//=3+var,
OpLine=8,//=4,
OpNoLine=317,//=1,
OpModuleProcessed=330//=2+var
}
enum AnnotationInstructions {
    OpDecorate=71//=3+var,
    OpMemberDecorate=72//=4+var,
    OpDecorationGroup=73//=2,
    OpGroupDecorate=74//=2+var,
    OpGroupMemberDecorate=75//=2+var,
    OpDecorateId=332//=3+var,
    OpDecorateString=5632//=4+var, (OpDecorateStringGOOGLE)
    OpMemberDecorateString=5633//=5+var, (OpMemberDecorateStringGOOGLE)
}
enum ExtensionsInstructions {
    OpExtension=10,//=2+var
    OpExtInstImport=11,//=3+var
    OpExtInst=12,//=5+var
OpExtInstWithForwardRefsKHR=4433,//=5+var
OpConditionalExtensionINTEL=6248//=3+var
}
enum Mode_SettingInstructions{
    OpMemoryModel=14,//=3//
    OpEntryPoint=15,//=4//
    OpExecutionMode=16,//=3//
    OpCapability=17,//=2//
    OpExecutionModeId=331,//=3//
    OpConditionalEntryPointINTEL=6249,//=5//
    OpConditionalCapabilityINTEL=6250,//=3//

}
enum Type_DeclarationInstructions
{
    OpTypeVoid=19,//=2
    OpTypeBool=20,//=2
    OpTypeInt=21,//=4
    OpTypeFloat=22,//=3
    OpTypeVector=23,//=4
    OpTypeMatrix=24,//=4+var,
    OpTypeImage=25,//=9
    OpTypeSampler=26,//=2
    OpTypeSampledImage=27,//=3
    OpTypeArray=28,//=4
    OpTypeRuntimeArray=29,//=3
    OpTypeStruct=30,//=2
    OpTypeOpaque=31,//=3
OpTypePointer=32,//=4
OpTypeFunction=33,//=3+var,
OpTypeEvent=34,//=2
OpTypeDeviceEvent=35,//=2
OpTypeReserveId=36,//=2
OpTypeQueue=37,//=2
OpTypePipe=38,//=3
OpTypeForwardPointer=39,//
OpTypePipeStorage=322,//=2
OpTypeNamedBarrier=327,//=2
OpTypeTensorARM=4163,//=3+var,
OpTypeGraphARM=4190,//=3+var,
OpTypeUntypedPointerKHR=4417,//=3,
OpTypeCooperativeMatrixKHR=4456,//=7,
OpTypeRayQueryKHR=4472,//=2,
OpTypeHitObjectNV=5281,//=2,
OpTypeCooperativeVectorNV=5288,//=4,
OpTypeAccelerationStructureKHR=5341,//=2,
OpTypeCooperativeMatrixNV=5358,//=6,
OpTypeTensorLayoutNV=5370,//=4,
OpTypeTensorViewNV=5371,//=4+var,
OpTypeBufferSurfaceINTEL=6086,//=3,
OpTypeStructContinuedINTEL=6090,//=1+var,
OpTypeTaskSequenceINTEL=6199//=2
}
enum Constant_CreationInstructions {
OpConstantTrue=41,//=3,
OpConstantFalse=42,//=3,
OpConstant=43,//=4+var,
OpConstantComposite=44,//=3+var
OpConstantSampler=45,//=6,
OpConstantNull=46,//=3,
OpSpecConstantTrue=48,//=3,
OpSpecConstantFalse=49,//=3,
OpSpecConstant=50,//=4+var,
OpSpecConstantComposite=51,//=3+var,
OpSpecConstantOp=52,//=4+var,
OpConstantCompositeReplicateEXT=4461,//=4,
OpSpecConstantCompositeReplicateEXT=4462,//=4,
OpConstantCompositeContinuedINTEL=6091,//=1+var,
OpSpecConstantCompositeContinuedINTEL=6092,//=1+var,
OpSpecConstantTargetINTEL=6251,//=4+var,
OpSpecConstantArchitectureINTEL=6252,//=7,
OpSpecConstantCapabilitiesINTEL=6253//=3+var
}
enum MemoryInstructions{
    OpVariable=59,//=4+var,
OpImageTexelPointer=60,//=6,
OpLoad=61,//=4+var,
OpStore=62,//=3+var,
OpCopyMemory=63,//=3+var,
OpCopyMemorySized=64,//=4+var,
OpAccessChain=65,//=4+var,
OpInBoundsAccessChain=66,//=4+var,
OpPtrAccessChain=67,//=5+var,
OpArrayLength=68,//=5,
OpGenericPtrMemSemantics=69,//=4,
OpInBoundsPtrAccessChain=70,//=5+var,
OpPtrEqual=401,//=5,
OpPtrNotEqual=402,//=5,
OpPtrDiff=403,//=5,
OpUntypedVariableKHR=4418,//=4,
OpUntypedAccessChainKHR=4419,//=5,
OpUntypedInBoundsAccessChainKHR=4420,//=5+var,
OpUntypedPtrAccessChainKHR=4423,//=6+var,
OpUntypedInBoundsPtrAccessChainKHR=4424,//=6+var,
OpUntypedArrayLengthKHR=4425,//=6,
OpUntypedPrefetchKHR=4426,//=3+var,
OpCooperativeMatrixLoadKHR=4457,//=5+var,
OpCooperativeMatrixStoreKHR=4458,//=4+var,
OpCooperativeVectorLoadNV=5302,//=5+var,
OpCooperativeVectorStoreNV=5303,//=4+var,
OpCooperativeMatrixLoadTensorNV=5367,//=8,
OpCooperativeMatrixStoreTensorNV=5368,//=6,
OpRawAccessChainNV=5398,//=7+var,
OpMaskedGatherINTEL=6428,//=7,
OpMaskedScatterINTEL=6429//=5
}
enum FunctionInstructions {
OpFunction=54,//=5
OpFunctionParameter=55,//=3
OpFunctionEnd=56,//=1
OpFunctionCall=57,//=4+var
OpCooperativeMatrixPerElementOpNV=5369//=5+var
} 

enum ImageInstructions {
OpSampledImage=86,//=5,
OpImageSampleImplicitLod=87,//=5+var
OpImageSampleExplicitLod=88,//=7+var
OpImageSampleDrefImplicitLod=89,//=6+var
OpImageSampleDrefExplicitLod=90,//=8+var
OpImageSampleProjImplicitLod=91,//=5+var
OpImageSampleProjExplicitLod=92,//=7+var
OpImageSampleProjDrefImplicitLod=93,//=6+var
OpImageSampleProjDrefExplicitLod=94,//=8+var
OpImageFetch=95,//=5+var
OpImageGather=96,//=6+var
OpImageDrefGather=97,//=6+var
OpImageRead=98,//=5+var
OpImageWrite=99,//=4+var
OpImage=100,//=4,
OpImageQueryFormat=101,//=4,
OpImageQueryOrder=102,//=4,
OpImageQuerySizeLod=103,//=5,
OpImageQuerySize=104,//=4,
OpImageQueryLod=105,//=5,
OpImageQueryLevels=106,//=4,
OpImageQuerySamples=107,//=4,
OpImageSparseSampleImplicitLod=305,//=5+var
OpImageSparseSampleExplicitLod=306,//=7+var
OpImageSparseSampleDrefImplicitLod=307,//=6+var
OpImageSparseSampleDrefExplicitLod=308,//=8+var
OpImageSparseFetch=313,//=5+var
OpImageSparseGather=314,//=6+var
OpImageSparseDrefGather=315,//=6+var
OpImageSparseTexelsResident=316,//=4,
OpImageSparseRead=320,//=5+var
OpColorAttachmentReadEXT=4160,//=4+var
OpDepthAttachmentReadEXT=4161,//=3+var
OpStencilAttachmentReadEXT=4162,//=3,
OpImageSampleWeightedQCOM=4480,//=6,
OpImageBoxFilterQCOM=4481,//=6,
OpImageBlockMatchSSDQCOM=4482,//=8,
OpImageBlockMatchSADQCOM=4483,//=8,
OpImageBlockMatchWindowSSDQCOM=4500,//=8,
OpImageBlockMatchWindowSADQCOM=4501,//=8,
OpImageBlockMatchGatherSSDQCOM=4502,//=8,
OpImageBlockMatchGatherSADQCOM=4503,//=8,
OpImageSampleFootprintNV=5283,//=7+var
OpConvertHandleToImageINTEL=6529,//=4,
OpConvertHandleToSamplerINTEL=6530,//=4,
OpConvertHandleToSampledImageINTEL=6531//=4
};
enum ConversionInstructions {
OpConvertFToU=109,//=4,
OpConvertFToS=110,//=4,
OpConvertSToF=111,//=4,
OpConvertUToF=112,//=4,
OpUConvert=113,//=4,
OpSConvert=114,//=4,
OpFConvert=115,//=4,
OpQuantizeToF16=116,//=4,
OpConvertPtrToU=117,//=4,
OpSatConvertSToU=118,//=4,
OpSatConvertUToS=119,//=4,
OpConvertUToPtr=120,//=4,
OpPtrCastToGeneric=121,//=4,
OpGenericCastToPtr=122,//=4,
OpGenericCastToPtrExplicit=123,//=5,
OpBitcast=124,//=4,
OpBitCastArrayQCOM=4497,//=4,
OpCooperativeMatrixConvertNV=5293,//=4,
OpCooperativeMatrixTransposeNV=5390,//=4,
OpConvertFToBF16INTEL=6116,//=4,
OpConvertBF16ToFINTEL=6117,//=4,
OpRoundFToTF32INTEL=6426//=4
}
enum CompositeInstructions {
OpVectorExtractDynamic=77,//=5,
OpVectorInsertDynamic=78,//=6,
OpVectorShuffle=79,//=5+var
OpCompositeConstruct=80,//=3,
OpCompositeExtract=81,//=43+var
OpCompositeInsert=82,//=53+var
OpCopyObject=83,//=4,
OpTranspose=84,//=4,
OpCopyLogical=400,//=4,
OpCompositeConstructReplicateEXT=4463,//=4,
OpCompositeConstructCoopMatQCOM=4540,//=4,
OpCompositeExtractCoopMatQCOM=4541,//=4,
OpExtractSubArrayQCOM=4542,//=5,
OpCompositeConstructContinuedINTEL=6096,//=3+var
OpConditionalCopyObjectINTEL=6254//=3+var
};
enum ArithmeticInstructions {
OpSNegate=126,//=4,
OpFNegate=127,//=4,
OpIAdd=128,//=5,
OpFAdd=129,//=5,
OpISub=130,//=5,
OpFSub=131,//=5,
OpIMul=132,//=5,
OpFMul=133,//=5,
OpUDiv=134,//=5,
OpSDiv=135,//=5,
OpFDiv=136,//=5,
OpUMod=137,//=5,
OpSRem=138,//=5,
OpSMod=139,//=5,
OpFRem=140,//=5,
OpFMod=141,//=5,
OpVectorTimesScalar=142,//=5,
OpMatrixTimesScalar=143,//=5,
OpVectorTimesMatrix=144,//=5,
OpMatrixTimesVector=145,//=5,
OpMatrixTimesMatrix=146,//=5,
OpOuterProduct=147,//=5,
OpDot=148,//=5,
OpIAddCarry=149,//=5,
OpISubBorrow=150,//=5,
OpUMulExtended=151,//=5,
OpSMulExtended=152,//=5,
OpSDot=4450,//=5+var 
OpUDot=4451,//=5+var 
OpSUDot=4452,//=5+var 
OpSDotAccSat=4453,//=6+var 
OpUDotAccSat=4454,//=6+var 
OpSUDotAccSat=4455,//=6+var 
OpCooperativeMatrixMulAddKHR=4459,//=6+var
OpCooperativeMatrixReduceNV=5366//=6
}
enum BitInstructions {
OpShiftRightLogical=194//=5,
OpShiftRightArithmetic=195//=5,
OpShiftLeftLogical=196//=5,
OpBitwiseOr=197//=5,
OpBitwiseXor=198//=5,
OpBitwiseAnd=199//=5,
OpNot=200//=4,
OpBitFieldInsert=201//=7,
OpBitFieldSExtract=202//=6,
OpBitFieldUExtract=203//=6,
OpBitReverse=204//=4,
OpBitCount=205//=4,
OpBitwiseFunctionINTEL=6242//=7
}
enum RelationalAndLogicalInstructions {
OpAny=154,//=4
OpAll=155,//=4
OpIsNan=156,//=4
OpIsInf=157,//=4
OpIsFinite=158,//=4
OpIsNormal=159,//=4
OpSignBitSet=160,//=4
OpLessOrGreater=161,//=5
OpOrdered=162,//=5
OpUnordered=163,//=5
OpLogicalEqual=164,//=5
OpLogicalNotEqual=165,//=5
OpLogicalOr=166,//=5
OpLogicalAnd=167,//=5
OpLogicalNot=168,//=4
OpSelect=169,//=6
OpIEqual=170,//=5
OpINotEqual=171,//=5
OpUGreaterThan=172,//=5
OpSGreaterThan=173,//=5
OpUGreaterThanEqual=174,//=5
OpSGreaterThanEqual=175,//=5
OpULessThan=176,//=5
OpSLessThan=177,//=5
OpULessThanEqual=178,//=5
OpSLessThanEqual=179,//=5
OpFOrdEqual=180,//=5
OpFUnordEqual=181,//=5
OpFOrdNotEqual=182,//=5
OpFUnordNotEqual=183,//=5
OpFOrdLessThan=184,//=5
OpFUnordLessThan=185,//=5
OpFOrdGreaterThan=186,//=5
OpFUnordGreaterThan=187,//=5
OpFOrdLessThanEqual=188,//=5
OpFUnordLessThanEqual=189,//=5
OpFOrdGreaterThanEqual=190,//=5
OpFUnordGreaterThanEqual=191,//=5


}

enum  DerivativeInstructions {
OpDPdx=207,//=4,
OpDPdy=208,//=4,
OpFwidth=209,//=4,
OpDPdxFine=210,//=4,
OpDPdyFine=211,//=4,
OpFwidthFine=212,//=4,
OpDPdxCoarse=213,//=4,
OpDPdyCoarse=214,//=4,
OpFwidthCoarse=215//=4
}
enum ControlFlowInstructions {
    OpPhi=245,//=3+var
    OpLoopMerge=246,//=4+var
    OpSelectionMerge=247,//=3
    OpLabel=248,//=2
    OpBranch=249,//=2
    OpBranchConditional=250,//=4+var
    OpSwitch=251,//=3+var
    OpKill=252,//=1
    OpReturn=253,//=1
    OpReturnValue=254,//=2
    OpUnreachable=255,//=1
    OpLifetimeStart=256,//=3
    OpLifetimeStop=257,//=3
    OpTerminateInvocation=4416,//=1
    OpDemoteToHelperInvocation=5380//=1 
}

enum AtomicInstructions {
OpAtomicLoad=227,//=6
OpAtomicStore=228,//=5
OpAtomicExchange=229,//=7
OpAtomicCompareExchange=230,//=9
OpAtomicCompareExchangeWeak=231,//=9
OpAtomicIIncrement=232,//=6
OpAtomicIDecrement=233,//=6
OpAtomicIAdd=234,//=7
OpAtomicISub=235,//=7
OpAtomicSMin=236,//=7
OpAtomicUMin=237,//=7
OpAtomicSMax=238,//=7
OpAtomicUMax=239,//=7
OpAtomicAnd=240,//=7
OpAtomicOr=241,//=7
OpAtomicXor=242,//=7
OpAtomicFlagTestAndSet=318,//=6
OpAtomicFlagClear=319,//=4
OpAtomicFMinEXT=5614,//=7
OpAtomicFMaxEXT=5615,//=7
OpAtomicFAddEXT=6035//=7
}
enum PrimitiveInstructions {
OpEmitVertex=218,//=1
OpEndPrimitive=219,//=1
OpEmitStreamVertex=220,//=2
OpEndStreamPrimitive=221//=2
};
enum BarrierInstructions {
OpControlBarrier=224,//=4,
OpMemoryBarrier=225,//=3,
OpNamedBarrierInitialize=328,//=4,
OpMemoryNamedBarrier=329,//=4,
OpControlBarrierArriveINTEL=6142,//=4,
OpControlBarrierWaitINTEL=6143//=4
}
enum GroupandSubgroupInstructions {
OpGroupAsyncCopy=259,//=9
OpGroupWaitEvents=260,//=4
OpGroupAll=261,//=5
OpGroupAny=262,//=5
OpGroupBroadcast=263,//=6
OpGroupIAdd=264,//=6
OpGroupFAdd=265,//=6
OpGroupFMin=266,//=6
OpGroupUMin=267,//=6
OpGroupSMin=268,//=6
OpGroupFMax=269,//=6
OpGroupUMax=270,//=6
OpGroupSMax=271,//=6
OpSubgroupBallotKHR=4421,//=4
OpSubgroupFirstInvocationKHR=4422,//=4
OpSubgroupAllKHR=4428,//=4
OpSubgroupAnyKHR=4429,//=4
OpSubgroupAllEqualKHR=4430,//=4
OpGroupNonUniformRotateKHR=4431,//=6+var
OpSubgroupReadInvocationKHR=4432,//=5
OpGroupIAddNonUniformAMD=5000,//=6
OpGroupFAddNonUniformAMD=5001,//=6
OpGroupFMinNonUniformAMD=5002,//=6
OpGroupUMinNonUniformAMD=5003,//=6
OpGroupSMinNonUniformAMD=5004,//=6
OpGroupFMaxNonUniformAMD=5005,//=6
OpGroupUMaxNonUniformAMD=5006,//=6
OpGroupSMaxNonUniformAMD=5007,//=6
OpSubgroupShuffleINTEL=5571,//=5
OpSubgroupShuffleDownINTEL=5572,//=6
OpSubgroupShuffleUpINTEL=5573,//=6
OpSubgroupShuffleXorINTEL=5574,//=5
OpSubgroupBlockReadINTEL=5575,//=4
OpSubgroupBlockWriteINTEL=5576,//=3
OpSubgroupImageBlockReadINTEL=5577,//=5
OpSubgroupImageBlockWriteINTEL=5578,//=4
OpSubgroupImageMediaBlockReadINTEL=5580,//=7
OpSubgroupImageMediaBlockWriteINTEL=5581,//=6
OpSubgroupBlockPrefetchINTEL=6221,//=3+var
OpSubgroup2DBlockLoadINTEL=6231,//=11
OpSubgroup2DBlockLoadTransformINTEL=6232,//=11
OpSubgroup2DBlockLoadTransposeINTEL=6233,//=11
OpSubgroup2DBlockPrefetchINTEL=6234,//=10
OpSubgroup2DBlockStoreINTEL=6235,//=11
OpSubgroupMatrixMultiplyAccumulateINTEL=6237,//=7+var
OpGroupIMulKHR=6401,//=6
OpGroupFMulKHR=6402,//=6
OpGroupBitwiseAndKHR=6403,//=6
OpGroupBitwiseOrKHR=6404,//=6
OpGroupBitwiseXorKHR=6405,//=6
OpGroupLogicalAndKHR=6406,//=6
OpGroupLogicalOrKHR=6407,//=6
OpGroupLogicalXorKHR=6408//=6
}
enum DeviceSideEnqueueInsturctions {
OpEnqueueMarker=291,//=7, 
OpEnqueueKernel=292,//=13,
OpGetKernelNDrangeSubGroupCount=293,//=8,
OpGetKernelNDrangeMaxSubGroupSize=294,//=8,
OpGetKernelWorkGroupSize=295,//=7,
OpGetKernelPreferredWorkGroupSizeMultiple=296,//=7,
OpRetainEvent=297,//=2,
OpReleaseEvent=298,//=2,
OpCreateUserEvent=299,//=3,
OpIsValidEvent=300,//=4,
OpSetUserEventStatus=301,//=3,
OpCaptureEventProfilingInfo=302,//=4,
OpGetDefaultQueue=303,//=3,
OpBuildNDRange=304,//=6,
OpGetKernelLocalSizeForSubgroupCount=325,//=8,
OpGetKernelMaxNumSubgroups=326//=7
};
enum PipeInstructions {
OpReadPipe=274,//=7,
OpWritePipe=275,//=7,
OpReservedReadPipe=276,//=9,
OpReservedWritePipe=277,//=9,
OpReserveReadPipePackets=278,//=7,
OpReserveWritePipePackets=279,//=7,
OpCommitReadPipe=280,//=5,
OpCommitWritePipe=281,//=5,
OpIsValidReserveId=282,//=4,
OpGetNumPipePackets=283,//=6,
OpGetMaxPipePackets=284,//=6,
OpGroupReserveReadPipePackets=285,//=8,
OpGroupReserveWritePipePackets=286,//=8,
OpGroupCommitReadPipe=287,//=6,
OpGroupCommitWritePipe=288,//=6,
OpConstantPipeStorage=323,//=6,
OpCreatePipeFromPipeStorage=324,//=4,
OpReadPipeBlockingINTEL=5946,//=5,
OpWritePipeBlockingINTEL=5947//=5
};

enum NonUniformInstructions {
OpGroupNonUniformElect=333,//=4,
OpGroupNonUniformAll=334,//=5,
OpGroupNonUniformAny=335,//=5,
OpGroupNonUniformAllEqual=336,//=5,
OpGroupNonUniformBroadcast=337,//=6,
OpGroupNonUniformBroadcastFirst=338,//=5,
OpGroupNonUniformBallot=339,//=5,
OpGroupNonUniformInverseBallot=340,//=5,
OpGroupNonUniformBallotBitExtract=341,//=6,
OpGroupNonUniformBallotBitCount=342,//=6,
OpGroupNonUniformBallotFindLSB=343,//=5,
OpGroupNonUniformBallotFindMSB=344,//=5,
OpGroupNonUniformShuffle=345,//=6,
OpGroupNonUniformShuffleXor=346,//=6,
OpGroupNonUniformShuffleUp=347,//=6,
OpGroupNonUniformShuffleDown=348,//=6,
OpGroupNonUniformIAdd=349,//=6,+var
OpGroupNonUniformFAdd=350,//=6,+var
OpGroupNonUniformIMul=351,//=6,+var
OpGroupNonUniformFMul=352,//=6,+var
OpGroupNonUniformSMin=353,//=6,+var
OpGroupNonUniformUMin=354,//=6,+var
OpGroupNonUniformFMin=355,//=6,+var
OpGroupNonUniformSMax=356,//=6,+var
OpGroupNonUniformUMax=357,//=6,+var
OpGroupNonUniformFMax=358,//=6,+var
OpGroupNonUniformBitwiseAnd=359,//=6,+var
OpGroupNonUniformBitwiseOr=360,//=6,+var
OpGroupNonUniformBitwiseXor=361,//=6,+var
OpGroupNonUniformLogicalAnd=362,//=6,+var
OpGroupNonUniformLogicalOr=363,//=6,+var
OpGroupNonUniformLogicalXor=364,//=6,+var
OpGroupNonUniformQuadBroadcast=365,//=6,
OpGroupNonUniformQuadSwap=366,//=6,
OpGroupNonUniformQuadAllKHR=5110,//=4,
OpGroupNonUniformQuadAnyKHR=5111,//=4,
OpGroupNonUniformPartitionNV=5296//=4
}

enum TensorInstructions {
    OpTensorReadARM=4164,//=5+var
    OpTensorWriteARM=4165,//=4+var
    OpTensorQuerySizeARM=4166//=5+var
}

enum GraphInstructions {
OpGraphConstantARM=4181,//=4,
OpGraphEntryPointARM=4182,//=3,
OpGraphARM=4183,//=3,
OpGraphInputARM=4184,//=4,
OpGraphSetOutputARM=4185,//=3,
OpGraphEndARM=4186//=1
}
enum ReservedInstructions {
OpTraceRayKHR=4445,//=12,
OpExecuteCallableKHR=4446,//=3,
OpConvertUToAccelerationStructureKHR=4447,//=4,
OpIgnoreIntersectionKHR=4448,//=1,
OpTerminateRayKHR=4449,//=1,
OpRayQueryInitializeKHR=4473,//=9,
OpRayQueryTerminateKHR=4474,//=2,
OpRayQueryGenerateIntersectionKHR=4475,//=3,
OpRayQueryConfirmIntersectionKHR=4476,//=2,
OpRayQueryProceedKHR=4477,//=4,
OpRayQueryGetIntersectionTypeKHR=4479,//=5,
OpFragmentMaskFetchAMD=5011,//=5,
OpFragmentFetchAMD=5012,//=6,
OpReadClockKHR=5056,//=4,
OpAllocateNodePayloadsAMDX=5074,//=6,
OpEnqueueNodePayloadsAMDX=5075,//=2,
OpTypeNodePayloadArrayAMDX=5076,//=3,
OpFinishWritingNodePayloadAMDX=5078,//=4,
OpNodePayloadArrayLengthAMDX=5090,//=4,
OpIsNodePayloadValidAMDX=5101,//=5,
OpConstantStringAMDX=5103,//=3,
OpSpecConstantStringAMDX=5104,//=3,
OpHitObjectRecordHitMotionNV=5249,//=15,
OpHitObjectRecordHitWithIndexMotionNV=5250,//=14,
OpHitObjectRecordMissMotionNV=5251,//=8,
OpHitObjectGetWorldToObjectNV=5252,//=4,
OpHitObjectGetObjectToWorldNV=5253,//=4,
OpHitObjectGetObjectRayDirectionNV=5254,//=4,
OpHitObjectGetObjectRayOriginNV=5255,//=4,
OpHitObjectTraceRayMotionNV=5256,//=14,
OpHitObjectGetShaderRecordBufferHandleNV=5257,//=4,
OpHitObjectGetShaderBindingTableRecordIndexNV=5258,//=4,
OpHitObjectRecordEmptyNV=5259,//=2,
OpHitObjectTraceRayNV=5260,//=13,
OpHitObjectRecordHitNV=5261,//=14,
OpHitObjectRecordHitWithIndexNV=5262,//=13,
OpHitObjectRecordMissNV=5263,//=7,
OpHitObjectExecuteShaderNV=5264,//=3,
OpHitObjectGetCurrentTimeNV=5265,//=4,
OpHitObjectGetAttributesNV=5266,//=3,
OpHitObjectGetHitKindNV=5267,//=4,
OpHitObjectGetPrimitiveIndexNV=5268,//=4,
OpHitObjectGetGeometryIndexNV=5269,//=4,
OpHitObjectGetInstanceIdNV=5270,//=4,
OpHitObjectGetInstanceCustomIndexNV=5271,//=4,
OpHitObjectGetWorldRayDirectionNV=5272,//=4,
OpHitObjectGetWorldRayOriginNV=5273,//=4,
OpHitObjectGetRayTMaxNV=5274,//=4,
OpHitObjectGetRayTMinNV=5275,//=4,
OpHitObjectIsEmptyNV=5276,//=4,
OpHitObjectIsHitNV=5277,//=4,
OpHitObjectIsMissNV=5278,//=4,
OpReorderThreadWithHitObjectNV=5279,//=2+var,
OpReorderThreadWithHintNV=5280,//=3,
OpCooperativeVectorMatrixMulNV=5289,//=12+var
OpCooperativeVectorOuterProductAccumulateNV=5290,//=7+var
OpCooperativeVectorReduceSumAccumulateNV=5291,//=4,
OpCooperativeVectorMatrixMulAddNV=5292,//=15+var
OpEmitMeshTasksEXT=5294,//=4+var
OpSetMeshOutputsEXT=5295,//=3,
OpWritePackedPrimitiveIndices4x8NV=5299,//=3,
OpFetchMicroTriangleVertexPositionNV=5300,//=8,
OpFetchMicroTriangleVertexBarycentricNV=5301,//=8,
OpReportIntersectionKHR=5334,//=5,
OpIgnoreIntersectionNV=5335,//=1,
OpTerminateRayNV=5336,//=1,
OpTraceNV=5337,//=12,
OpTraceMotionNV=5338,//=13,
OpTraceRayMotionNV=5339,//=13,
OpRayQueryGetIntersectionTriangleVertexPositionsKHR=5340,//=5,
OpExecuteCallableNV=5344,//=3,
OpRayQueryGetClusterIdNV=5345,//=5,
OpHitObjectGetClusterIdNV=5346,//=4,
OpCooperativeMatrixLoadNV=5359,//=6+var
OpCooperativeMatrixStoreNV=5360,//=5+var
OpCooperativeMatrixMulAddNV=5361,//=6,
OpCooperativeMatrixLengthNV=5362,//=4,
OpBeginInvocationInterlockEXT=5364,//=1,
OpEndInvocationInterlockEXT=5365,//=1,
OpCreateTensorLayoutNV=5372,//=3,
OpTensorLayoutSetDimensionNV=5373,//=4+var
OpTensorLayoutSetStrideNV=5374,//=4+var
OpTensorLayoutSliceNV=5375,//=4+var
OpTensorLayoutSetClampValueNV=5376,//=5,
OpCreateTensorViewNV=5377,//=3,
OpTensorViewSetDimensionNV=5378,//=4+var
OpTensorViewSetStrideNV=5379,//=4+var
OpIsHelperInvocationEXT=5381,//=3,
OpTensorViewSetClipNV=5382,//=8,
OpTensorLayoutSetBlockSizeNV=5384,//=4+var
OpConvertUToImageNV=5391,//=4,
OpConvertUToSamplerNV=5392,//=4,
OpConvertImageToUNV=5393,//=4,
OpConvertSamplerToUNV=5394,//=4,
OpConvertUToSampledImageNV=5395,//=4,
OpConvertSampledImageToUNV=5396,//=4,
OpSamplerImageAddressingModeNV=5397,//=2,
OpRayQueryGetIntersectionSpherePositionNV=5427,//=5,
OpRayQueryGetIntersectionSphereRadiusNV=5428,//=5,
OpRayQueryGetIntersectionLSSPositionsNV=5429,//=5,
OpRayQueryGetIntersectionLSSRadiiNV=5430,//=5,
OpRayQueryGetIntersectionLSSHitValueNV=5431,//=5,
OpHitObjectGetSpherePositionNV=5432,//=4,
OpHitObjectGetSphereRadiusNV=5433,//=4,
OpHitObjectGetLSSPositionsNV=5434,//=4,
OpHitObjectGetLSSRadiiNV=5435,//=4,
OpHitObjectIsSphereHitNV=5436,//=4,
OpHitObjectIsLSSHitNV=5437,//=4,
OpRayQueryIsSphereHitNV=5438,//=5,
OpRayQueryIsLSSHitNV=5439,//=5,
OpUCountLeadingZerosINTEL=5585,//=4,
OpUCountTrailingZerosINTEL=5586,//=4,
OpAbsISubINTEL=5587,//=5,
OpAbsUSubINTEL=5588,//=5,
OpIAddSatINTEL=5589,//=5,
OpUAddSatINTEL=5590,//=5,
OpIAverageINTEL=5591,//=5,
OpUAverageINTEL=5592,//=5,
OpIAverageRoundedINTEL=5593,//=5,
OpUAverageRoundedINTEL=5594,//=5,
OpISubSatINTEL=5595,//=5,
OpUSubSatINTEL=5596,//=5,
OpIMul32x16INTEL=5597,//=5,
OpUMul32x16INTEL=5598,//=5,
OpLoopControlINTEL=5887,//=1,
OpFPGARegINTEL=5949,//=4,
OpRayQueryGetRayTMinKHR=6016,//=4,
OpRayQueryGetRayFlagsKHR=6017,//=4,
OpRayQueryGetIntersectionTKHR=6018,//=5,
OpRayQueryGetIntersectionInstanceCustomIndexKHR=6019,//=5,
OpRayQueryGetIntersectionInstanceIdKHR=6020,//=5,
OpRayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetKHR=6021,//=5,
OpRayQueryGetIntersectionGeometryIndexKHR=6022,//=5,
OpRayQueryGetIntersectionPrimitiveIndexKHR=6023,//=5,
OpRayQueryGetIntersectionBarycentricsKHR=6024,//=5,
OpRayQueryGetIntersectionFrontFaceKHR=6025,//=5,
OpRayQueryGetIntersectionCandidateAABBOpaqueKHR=6026,//=4,
OpRayQueryGetIntersectionObjectRayDirectionKHR=6027,//=5,
OpRayQueryGetIntersectionObjectRayOriginKHR=6028,//=5,
OpRayQueryGetWorldRayDirectionKHR=6029,//=4,
OpRayQueryGetWorldRayOriginKHR=6030,//=4,
OpRayQueryGetIntersectionObjectToWorldKHR=6031,//=5,
OpRayQueryGetIntersectionWorldToObjectKHR=6032,//=5,
OpTaskSequenceCreateINTEL=6163,//=8,
OpTaskSequenceAsyncINTEL=6164,//=2+var
OpTaskSequenceGetINTEL=6165,//=4,
OpTaskSequenceReleaseINTEL=6166//=2
}

}