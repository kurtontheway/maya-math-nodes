// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TAttrType, typename TClass, const char* TTypeName>
class InverseNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(0.0), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = getAttribute<TAttrType>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, inputValue.inverse());
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute InverseNode<TAttrType, TClass, TTypeName>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute InverseNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define INVERSE_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public InverseNode<AttrType, NodeName, name##NodeName> {};

INVERSE_NODE(MMatrix, InverseMatrix);
INVERSE_NODE(MQuaternion, InverseQuaternion);
