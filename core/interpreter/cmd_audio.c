//
// Copyright 2018 Timo Kloss
//
// This file is part of LowRes NX.
//
// LowRes NX is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LowRes NX is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LowRes NX.  If not, see <http://www.gnu.org/licenses/>.
//

#include "cmd_audio.h"
#include "core.h"
#include "interpreter_utils.h"
#include <assert.h>
#include <math.h>

enum ErrorCode cmd_VOICE(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // VOICE
    ++interpreter->pc;
    
    // n value
    struct TypedValue nValue = itp_evaluateNumericExpression(core, 0, NUM_VOICES - 1);
    if (nValue.type == ValueTypeError) return nValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // frequency value
    struct TypedValue fValue = itp_evaluateOptionalNumericExpression(core, 0, 65535);
    if (fValue.type == ValueTypeError) return fValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // volume value
    struct TypedValue volValue = itp_evaluateOptionalNumericExpression(core, 0, 255);
    if (volValue.type == ValueTypeError) return volValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // pulse width value
    struct TypedValue pwValue = itp_evaluateOptionalNumericExpression(core, 0, 255);
    if (pwValue.type == ValueTypeError) return pwValue.v.errorCode;
    
    if (interpreter->pass == PassRun)
    {
        int n = nValue.v.floatValue;
        struct Voice *voice = &core->machine->audioRegisters.voices[n];
        if (fValue.type != ValueTypeNull)
        {
            int f = fValue.v.floatValue;
            voice->frequencyLow = f & 0xFF;
            voice->frequencyHigh = f >> 8;
        }
        if (volValue.type != ValueTypeNull)
        {
            voice->volume = volValue.v.floatValue;
        }
        if (pwValue.type != ValueTypeNull)
        {
            voice->pulseWidth = pwValue.v.floatValue;
        }
        
        if (!core->machine->audioRegisters.attr.audioEnabled)
        {
            core->machine->audioRegisters.attr.audioEnabled = 1;
            delegate_controlsDidChange(core);
        }
    }
    
    return itp_endOfCommand(interpreter);
}

enum ErrorCode cmd_VOICE_A(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // VOICE.A
    ++interpreter->pc;
    
    // n value
    struct TypedValue nValue = itp_evaluateNumericExpression(core, 0, NUM_VOICES - 1);
    if (nValue.type == ValueTypeError) return nValue.v.errorCode;
    
    struct Voice *voice = NULL;
    if (interpreter->pass == PassRun)
    {
        int n = nValue.v.floatValue;
        voice = &core->machine->audioRegisters.voices[n];
    }
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    union VoiceAttributes attr;
    if (voice) attr = voice->attr; else attr.value = 0;
    
    // attr value
    struct TypedValue aValue = itp_evaluateVoiceAttributes(core, attr);
    if (nValue.type == ValueTypeError) return nValue.v.errorCode;
    
    if (interpreter->pass == PassRun)
    {
        voice->attr.value = aValue.v.floatValue;
    }
    
    return itp_endOfCommand(interpreter);
}

enum ErrorCode cmd_VOICE_EG(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // VOICE.EG
    ++interpreter->pc;
    
    // n value
    struct TypedValue nValue = itp_evaluateNumericExpression(core, 0, NUM_VOICES - 1);
    if (nValue.type == ValueTypeError) return nValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // attack value
    struct TypedValue attValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (attValue.type == ValueTypeError) return attValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // decay value
    struct TypedValue decValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (decValue.type == ValueTypeError) return decValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // sustain value
    struct TypedValue susValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (susValue.type == ValueTypeError) return susValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // release value
    struct TypedValue relValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (relValue.type == ValueTypeError) return relValue.v.errorCode;
    
    if (interpreter->pass == PassRun)
    {
        int n = nValue.v.floatValue;
        struct Voice *voice = &core->machine->audioRegisters.voices[n];
        if (attValue.type != ValueTypeNull)
        {
            voice->envA = attValue.v.floatValue;
        }
        if (decValue.type != ValueTypeNull)
        {
            voice->envD = decValue.v.floatValue;
        }
        if (susValue.type != ValueTypeNull)
        {
            voice->envS = susValue.v.floatValue;
        }
        if (relValue.type != ValueTypeNull)
        {
            voice->envR = relValue.v.floatValue;
        }
    }
    
    return itp_endOfCommand(interpreter);
}

enum ErrorCode cmd_VOICE_LFO(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // VOICE.LFO
    ++interpreter->pc;
    
    // n value
    struct TypedValue nValue = itp_evaluateNumericExpression(core, 0, NUM_VOICES - 1);
    if (nValue.type == ValueTypeError) return nValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // rate value
    struct TypedValue rateValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (rateValue.type == ValueTypeError) return rateValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // osc amount value
    struct TypedValue oscValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (oscValue.type == ValueTypeError) return oscValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // vol amount value
    struct TypedValue volValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (volValue.type == ValueTypeError) return volValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // pw amount value
    struct TypedValue pwValue = itp_evaluateOptionalNumericExpression(core, 0, 15);
    if (pwValue.type == ValueTypeError) return pwValue.v.errorCode;
    
    if (interpreter->pass == PassRun)
    {
        int n = nValue.v.floatValue;
        struct Voice *voice = &core->machine->audioRegisters.voices[n];
        if (rateValue.type != ValueTypeNull)
        {
            voice->lfoFrequency = rateValue.v.floatValue;
        }
        if (oscValue.type != ValueTypeNull)
        {
            voice->lfoOscAmount = oscValue.v.floatValue;
        }
        if (volValue.type != ValueTypeNull)
        {
            voice->lfoVolAmount = volValue.v.floatValue;
        }
        if (pwValue.type != ValueTypeNull)
        {
            voice->lfoPWAmount = pwValue.v.floatValue;
        }
    }
    
    return itp_endOfCommand(interpreter);
}

enum ErrorCode cmd_VOICE_LFO_A(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // VOICE.LFO.A
    ++interpreter->pc;
    
    // n value
    struct TypedValue nValue = itp_evaluateNumericExpression(core, 0, NUM_VOICES - 1);
    if (nValue.type == ValueTypeError) return nValue.v.errorCode;
    
    struct Voice *voice = NULL;
    if (interpreter->pass == PassRun)
    {
        int n = nValue.v.floatValue;
        voice = &core->machine->audioRegisters.voices[n];
    }
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    union LFOAttributes attr;
    if (voice) attr = voice->lfoAttr; else attr.value = 0;
    
    // attr value
    struct TypedValue attrValue = itp_evaluateLFOAttributes(core, attr);
    if (attrValue.type == ValueTypeError) return attrValue.v.errorCode;
    
    if (interpreter->pass == PassRun)
    {
        voice->lfoAttr.value = attrValue.v.floatValue;
    }
    
    return itp_endOfCommand(interpreter);
}

enum ErrorCode cmd_PLAY(struct Core *core)
{
    struct Interpreter *interpreter = core->interpreter;
    
    // PLAY
    ++interpreter->pc;
    
    // n value
    struct TypedValue nValue = itp_evaluateNumericExpression(core, 0, NUM_VOICES - 1);
    if (nValue.type == ValueTypeError) return nValue.v.errorCode;
    
    // comma
    if (interpreter->pc->type != TokenComma) return ErrorExpectedComma;
    ++interpreter->pc;
    
    // pitch value
    struct TypedValue pValue = itp_evaluateNumericExpression(core, 0, 95);
    if (pValue.type == ValueTypeError) return pValue.v.errorCode;
    
    if (interpreter->pass == PassRun)
    {
        int n = nValue.v.floatValue;
        struct Voice *voice = &core->machine->audioRegisters.voices[n];
        
        if (pValue.v.floatValue > 0.0)
        {
            int f = 16.0 * 440.0 * pow(2.0, (pValue.v.floatValue - 58.0) / 12.0);
            voice->frequencyLow = f & 0xFF;
            voice->frequencyHigh = f >> 8;
            
            voice->volume = 15;
            voice->attr.gate = 1;
            audio_onVoiceAttrChange(core, n);
        }
        else
        {
            voice->attr.gate = 0;
        }
        
        if (!core->machine->audioRegisters.attr.audioEnabled)
        {
            core->machine->audioRegisters.attr.audioEnabled = 1;
            delegate_controlsDidChange(core);
        }
    }
    
    return itp_endOfCommand(interpreter);
}
